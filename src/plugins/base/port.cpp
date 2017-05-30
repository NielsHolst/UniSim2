#include "box.h"
#include "general.h"
#include "path.h"
#include "port.h"
#include "track.h"
#include "vectorize.h"

namespace base {

QVector<Port*> Port::_index;

// Configure

Port::Port(QString name, QObject *parent)
    : QObject(parent), _valuePtr(0), _valueType(Null), _mode(PortMode::Default),
      _portValueStep(ComputationStep::Start),
      _importPath(""), _importPortMustExist(true), _importsResolved(false),
      _access(PortAccess::Input),
      _notReferenced(false), _reset(false), _valueOverridden(false),
      _isBlind(false)
{
    Class(Port);
    setObjectName(name);
    Box *boxParent = dynamic_cast<Box*>(parent);
    if (boxParent)
        boxParent->addPort(this);
    _id = _index.size();
    _index << this;
}

Port& Port::equals(const char *value) {
    return equals(QString(value));
}

Port& Port::equals(QStringList value) {
    return equals(value.toVector());
}

Port& Port::imports(QString pathToPort) {
    if (_notReferenced)
        return equals(pathToPort);
    _mode = PortMode::Referenced;
    _portValueStep = environment().computationStep();
    _importPath = pathToPort;
    _importPortMustExist = true;
    checkValueOverridden();
    return help("Defaults to " + pathToPort);
}

Port& Port::importsMaybe(QString pathToPort) {
    if (_notReferenced)
        return equals(pathToPort);
    _mode = PortMode::MaybeReferenced;
    _portValueStep = environment().computationStep();
    _importPath = pathToPort;
    _importPortMustExist = false;
    checkValueOverridden();
    return help("Defaults to " + pathToPort + " (if it exists)");
}

void Port::checkValueOverridden() {
    Box *boxParent = dynamic_cast<Box*>(parent());
//    if (!boxParent)
//        ThrowException("Parent of Box class expected").context(this);
    _valueOverridden =  boxParent && !boxParent->underConstruction();
}

Port& Port::access(PortAccess acc) {
    _access = acc;
    return *this;
}

Port& Port::notReferenced() {
    _notReferenced = true;
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

QStringList Port::attributes() {
    static QStringList attr;
    if (attr.isEmpty()) {
        attr << "format" << "page" << "plot" << "label" << "transform";
    }
    return attr;
}

// Set attributes

#define CASE_SET_ATTRIBUTE(X,Y) \
if (name == #X) { \
    _attributes.X = Y; \
    return *this; \
}

Port& Port::attribute(QString name, QString value) {
    CASE_SET_ATTRIBUTE(format, value)
    CASE_SET_ATTRIBUTE(label, value)
    CASE_SET_ATTRIBUTE(help, value)
    if (name == "transform") {
        _attributes.transform = value;
        _attributes.portTransform = convert<PortTransform>(value);
        return *this;
    }
    ThrowException("Unknown attribute").value(name).context(this);
}

#define SET_ATTRIBUTE(X) \
Port& Port::X(QString value) { \
    return attribute(#X, value); \
}

SET_ATTRIBUTE(format)
SET_ATTRIBUTE(label)
SET_ATTRIBUTE(transform)
SET_ATTRIBUTE(help)

Port& Port::transform(PortTransform tr) {
    return attribute("transform", convert<QString>(tr));
}

Port& Port::isBlind(bool on) {
    _isBlind = on;
    return *this;
}

QString Port::name() const {
    return objectName();
}

QString Port::fullName() const {
    QString path = parent() ? base::fullName(parent()) : QString();
    return path + "[" + name() + "]";
}

int Port::id() const {
    return _id;
}

// Get attributes

#define CASE_GET_ATTRIBUTE(X) if (name == #X) return _attributes. X

QString Port::attribute(QString name) const {
    CASE_GET_ATTRIBUTE(format);
    CASE_GET_ATTRIBUTE(label);
    CASE_GET_ATTRIBUTE(transform);
    CASE_GET_ATTRIBUTE(help);
    ThrowException("Unknown attribute").value(name).context(this);
}

#define GET_ATTRIBUTE(X) \
QString Port::X() const { \
    return _attributes. X; \
}

GET_ATTRIBUTE(format)
GET_ATTRIBUTE(label)
GET_ATTRIBUTE(help)

bool Port::isBlind() const {
    return _isBlind;
}

PortTransform Port::transform() const {
    return _attributes.portTransform;
}

// Change

namespace {
    PortType deduceTypeFromImportType(PortType importType, PortTransform transform) {
        if (isScalar(importType))
            return importType;

        PortType type{Null};
        switch (transform) {
            case Identity:
                type = importType;
                break;
            case Sum:
            case Prod:
            case Mean:
            case Min:
            case Max:
            case All:
            case Any:
                type = asScalar(importType);
                break;
            case Copy:
            case Split:
                ThrowException("Transform cannot be applied on a vector").value(transform);
        }
        return type;
    }
}

void Port::resolveImports() {
    // Only resolve one
    if (_importsResolved)
        ThrowException("Unexpected: Imports already resolved").context(this);
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

    // Missing import ports may be an error, otherwise accept and clear the import path
    if (_importPorts.isEmpty()) {
        if (_importPortMustExist)
            ThrowException("No matching import ports found").value(_importPath).context(this);
        else {
            _importPath.clear();
            return;
        }
    }

    // Import port must have a type
    _importType = commonType(_importPorts);
    if (_importType == Null)
        ThrowException("Import port is of unknown type").value(_importPath).context(this);

    // Deduce value type from import type (this happens of port is an extra, added port)
    if (_valueType == Null) {
        _valueType = deduceTypeFromImportType(_importType, transform());
        if (_valueType == Null)
            ThrowException("Unexpected error: Type of imported value is Null");
    }
}

void Port::addExportPort(Port *port) {
    if (!_exportPorts.contains(port))
        _exportPorts << port;
}

void Port::reset() {
    if (!hasImport() && _reset)
        base::initialize(_valueType, _valuePtr, this);
}

void Port::copyFromImport() {
    if (!hasImport())
        return;
    assign(_importPorts);
}

void Port::assign(const QVector<Port*> &sources) {
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

bool Port::hasValue() const {
    return _valuePtr != 0;
}

#define CASE_VALUE_SIZE(X,Y) \
    case Y##Vector: \
        return reinterpret_cast<QVector<X>*>(_valuePtr)->size(); \
        break

int Port::valueSize() const {
    switch (_valueType) {
    CASE_VALUE_SIZE(bool, Bool);
    CASE_VALUE_SIZE(char, Char);
    CASE_VALUE_SIZE(int, Int);
    CASE_VALUE_SIZE(long int, LongInt);
    CASE_VALUE_SIZE(long long int, LongLongInt);
    CASE_VALUE_SIZE(float, Float);
    CASE_VALUE_SIZE(double, Double);
    CASE_VALUE_SIZE(long double, LongDouble);
    CASE_VALUE_SIZE(QString, String);
    CASE_VALUE_SIZE(QDate, Date);
    CASE_VALUE_SIZE(QDateTime, DateTime);
    CASE_VALUE_SIZE(QTime, Time);
    default:
        return 1;
    }
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
    }
    return s;
}

template <> const void* Port::valuePtr() const {
    return _valuePtr;
}

PortType Port::type() const {
    return _valueType;
}

PortAccess Port::access() const {
    return _access;
}

Track::Order Port::track(PortFilter filter) {
    return Track::takeOrder(this, filter);
}

bool Port::hasImport() const {
    return _mode!=PortMode::Fixed && !_importPath.isEmpty();
}

bool Port::hasDistribution() const {
    return Path("children::*<Distribution>", this).resolveMaybeOne<>(this);
}

bool Port::isValueOverridden() const {
    return _valueOverridden;
}

QString Port::importPath() const {
    return _importPath;
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
    QString prefix = (access() == PortAccess::Input) ? "." : "//>";
//    if (access() == PortAccess::Input) {
//        prefix = (constructionStep() == ComputationStep::Amend) ? "//." : ".";
//    }
//    else{
//        prefix = "//>";
//    }
    bool showAmended = false;
    if (constructionStep() == ComputationStep::Amend) {
        Box *myBox = dynamic_cast<Box*>(parent());
        showAmended = (myBox && myBox->constructionStep() != ComputationStep::Amend);
    }
    QString postfix = showAmended ? " //amended" : "";

    QString equalSign = (access() == PortAccess::Input) ? " = " : " == ";
    QString assignment = hasImport() ? _importPath : valueAsString();
    if (_valueType == Char || _valueType == String)
        assignment = "\"" + assignment + "\"";

    text << fill
         << prefix << objectName()
         << equalSign << assignment
         << postfix
//         << " // " << convert<QString>(_portValueStep)
         << "\n";
}

PortType Port::commonType(const QVector<Port*> &ports) {
    PortType commonType = ports.at(0)->_valueType;
    for (Port *port : ports) {
        if (port->_valueType != commonType) {
            commonType = Null;
            break;
        }
    }
    return commonType;
}

void Port::clearIndex() {
    _index.clear();
}

Port* Port::find(int id) {
    if (id > _index.size())
        ThrowException("Unexpected error: Index out of bounds").
                value(id).value2(_index.size());
    return _index[id];
}

QString Port::dump() {
    QString s;
    for (Port *port : _index) {
        s += QString::number(port->id()) + ": " + port->fullName() + "\n";
    }
    return s;
}

}

