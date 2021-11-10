/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "general.h"
#include "path.h"
#include "port.h"
#include "track.h"
#include "vectorize.h"

#include "dialog.h"

namespace base {

QVector<Port*> Port::_index;

Port::Port(QString name, QObject *parent)
    : QObject(parent), _mode(PortMode::Default),
      _importPath(""), _importsResolved(false),
      _access(PortAccess::Input),
      _isReference(false), _reset(false), _valueOverridden(false),
      _isBlind(false)
{
    Class(Port);
    setObjectName(name);
    Box *boxParent = dynamic_cast<Box*>(parent);
    if (boxParent)
        boxParent->addPort(this);
    _evaluationOrder = _index.size();
    _index << this;
}

Port& Port::equals(const char *value) {
    return equals(QString(value));
}

Port& Port::imports(QString pathToPort, Caller caller) {
    _mode = PortMode::Referenced;
    _importCaller = caller;
    return help("Defaults to " + pathToPort);
}

Port& Port::access(PortAccess acc) {
    _access = acc;
    return *this;
}

Port& Port::zeroAtReset() {
    _reset = true;
    return *this;
}

Port& Port::noReset() {
    _reset = false;
    return *this;
}

// Set attributes


Port& Port::help(QString value) {
    _help = value;
    return *this;
}

Port& Port::unit(QString value) {
    _unit = value;
    return *this;
}

Port& Port::isBlind(bool on) {
    _isBlind = on;
    return *this;
}

// Get attributes

QString Port::help() const {
    return _help;
}

QString Port::unit() const {
    return _unit;
}

bool Port::isBlind() const {
    return _isBlind;
}

// Names and id

QString Port::name() const {
    return objectName();
}

QString Port::fullName() const {
    QString path = parent() ? base::fullName(parent()) : QString();
    return path + "[" + name() + "]";
}

int Port::id() const {
    return _evaluationOrder;
}

// Change

void Port::enumerate(int &number) {
    _evaluationOrder = number++;
}

void Port::resolveImports() {
    // Only resolve once
    if (_importsResolved)
        return;
    _importsResolved = true;

    // No imports; nothing to do
    if (!hasImport())
        return;

    // Find import ports defined by the import path
    Box *context = boxParent();
    Path path = Path(_importPath, context);
    _importPorts = path.resolveMany<Port>();

    // Notify exporters
    for (Port *exporter : _importPorts)
        exporter->addExportPort(this);

    // Missing import ports is OK only if this is a vector
    if (_importPorts.isEmpty()) {
        if (isVector(type())) {
            _importPorts.clear();
            return;
        }
        else {
            ThrowException("No imports found").value(_importPath).context(context);
        }
    }

    // Import port must have a type
    _importType = commonType(_importPorts);
    if (_importType == Null)
        ThrowException("Import port is of unknown type").value(_importPath).context(this).caller(_importCaller);

    // Deduce value type from import type (this happens of port is an extra, added port)
    if (_valueType == Null) {
        _valueType = deduceTypeFromImportType(_importType, transform());
        if (_valueType == Null)
            ThrowException("Unexpected error: Type of imported value is Null").value(_importPath).context(this).caller(_importCaller);
    }

    // Set unit to import's
    unit(_importPorts.at(0)->unit());
}

void Port::resolveImportsAgain() {
    _importsResolved = false;
    resolveImports();
}

void Port::addExportPort(Port *port) {
    if (!_exportPorts.contains(port))
        _exportPorts << port;
}

void Port::reset() {
    if (!hasImport() && _reset)
        base::initialize(_valueType, _valuePtr, this);
}

void Port::update() {
    // If port is imported then tell the parent of the imported port to update the imported value
    if (!_importPath.isEmpty()) {
        PortMode saveMode = _mode;
        if (_importPorts.size() > 1)
            ThrowException("Port::update only accepts scalar import").context(this);
        _importPorts.at(0)->boxParent()->update(this);
        _mode = saveMode;
    }
}

void Port::copyFromImport() {
    if (hasImport())
        assign(_importPorts);
}

void Port::checkSelfImport() {
    if (hasImport()) {
        Port *me = const_cast<Port*>(this);
        for (Port *source : _importPorts) {
            const Box *sourceParent = const_cast<const Box*>(source->boxParent());
            const Box *myParent     = const_cast<const Box*>(me->boxParent());
            if (sourceParent == myParent) {
                ThrowException("Ports in the same box should not import one another").
                        value(fullName()).
                        value2(source->fullName()).
                        context(this);
            }
        }
    }
}

void Port::assign(const QVector<Port*> &sources) {
    // Nothing to do (can happen if a vector port has no sources)
    if (sources.isEmpty())
        return;
    // Create buffer for value if necessary
    if (!_valuePtr) {
        if (_valueType == Null) {
            _valueType = sources.at(0)->type();
            if (sources.size() > 1 && transform() != Identity)
                _valueType = asVector(_valueType);
        }
        _valuePtr = portBuffer(this).createBuffer(_valueType);
    }
    // Notify sources' parents about imminent import
    for (const Port *source : sources)
        const_cast<Port*>(source)->boxParent()->import();
    // Assign as scalar or vector
    if (sources.size() == 1) {
        const Port *source = sources.at(0);
        base::assign(_valueType, _valuePtr, source->_valueType, source->_valuePtr, transform(), this);
    }
    else {
        const void *sourceVector = vectorize(_importType, sources);
        base::assign(_valueType, _valuePtr, asVector(_importType), sourceVector, transform(), this);
    }
}

void Port::format(PortType type) {
    switch (type) {
        case Date:
        case DateVector:
            format("ymd");
            break;
        case Time:
        case TimeVector:
            format("HMS");
            break;
        case DateTime:
        case DateTimeVector:
            format("ymdHMS");
            break;
        default:
            format("NA");
    }
}

// Access

Box *Port::boxParent() {
    Box *par = dynamic_cast<Box*>(parent());
    if (!par)
        ThrowException("Application error: Port has no Box parent").context(this);
    return par;
}

expression::Operand Port::value() const {
    return _value;
}

QString Port::valueAsString() const {
    QString s;
    if (isVector(type())) {
        QVector<QString> vec = value<QVector<QString>>();
        QStringList list(vec.toList());
        s = "(" + list.join(" ") + ")";
    }
    else {
        s = value<QString>();
        if ((type()==Float || type()==Double || type()==LongDouble)
             && !s.contains("inf") && !s.contains(".") && !s.contains("e"))
            s = s + ".0";
    }
    return s;
}

void Port::verifyValue() const {
    if (valueAsString().startsWith("nan")) {
        ThrowException("Value is not a number").context(this);
    }
    else if (valueAsString() == "NA") {
        ThrowException("Value is NA").context(this);
    }
}

expression::Operand::Type Port::type() const {
    return _value.type();
}

PortAccess Port::access() const {
    return _access;
}

Track::Order Port::track(PortFilter filter) {
    return Track::takeOrder(this, filter);
}

bool Port::hasImport() const {
    return _unresolvedExpression.containsPortPaths();
}

QVector<Port*> Port::importPorts() const {
    return _importPorts;
}

QVector<Port*> Port::exportPorts() const {
    return _exportPorts;
}

QStringList Port::warnings() const {
    return _warnings;
}

void Port::toText(QTextStream &text, int indentation) const {
    QString fill;
    fill.fill(' ', indentation);
    QString prefix =
            (access() == PortAccess::Input) ?
                (isBlind() ? "+" : "." ) : "//~";
    bool showAmended = false;
    if (constructionStep() == ComputationStep::Amend) {
        Box *myBox = dynamic_cast<Box*>(parent());
        showAmended = (myBox && myBox->constructionStep() != ComputationStep::Amend);
    }
    QString postfix = showAmended ? " //amended" : "";

    QString equalSign = (access() == PortAccess::Input) ? " = " : " == ";
    QString assignment;
    if (hasImport())
        assignment = _importPath;
    else {
        assignment = valueAsString();
        if (_valueType == Char || _valueType == String || _valueType == DateTime)
            assignment = "\"" + assignment + "\"";
    }

    text << fill
         << prefix << objectName()
         << equalSign << assignment
         << postfix
//         << " // " << convert<QString>(_portValueStep)
         << "\n";
}

}

