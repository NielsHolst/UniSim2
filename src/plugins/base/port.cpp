/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "general.h"
#include "path.h"
#include "port.h"
#include "track.h"


namespace base {

Port::Port(QString name, QObject *parent)
    : QObject(parent),
      _hasDefaultValue(true),
      _doReset(true),
      _isExtraneous(false),
      _access(PortAccess::Input)
{
    Class(Port);
    setObjectName(name);
    Box *boxParent = dynamic_cast<Box*>(parent);
    if (boxParent)
        boxParent->addPort(this);
}

Port& Port::doReset() {
    _doReset = true;
    return *this;
}

Port& Port::noReset() {
    _doReset = false;
    return *this;
}

Port& Port::access(PortAccess acc) {
    _access = acc;
    return *this;
}

Port& Port::unit(QString value) {
    _unit = value;
    return *this;
}

Port& Port::help(QString value) {
    _help = value;
    return *this;
}

Port& Port::isExtraneous() {
    _isExtraneous = true;
}

template <>
Port& Port::equals(const char *fixedValue) {
    return equals(QString(fixedValue));
}

template <>
Port& Port::equals(Value value) {
    _value = value;
}

Port& Port::imports(QString pathToPort, Caller caller) {
    _expression.push(pathToPort);
    _importCaller = caller;
    return help("Defaults to " + pathToPort);
}

//
// Query
//

Box *Port::boxParent() {
    Box *par = dynamic_cast<Box*>(parent());
    if (!par)
        ThrowException("Application error: Port has no Box parent").context(this);
    return par;
}

QString Port::name() const {
    return objectName();
}

QString Port::fullName() const {
    QString path = parent() ? base::fullName(parent()) : QString();
    return path + "[" + name() + "]";
}

PortAccess Port::access() const {
    return _access;
}

QString Port::unit() const {
    return _unit;
}

QString Port::help() const {
    return _help;
}

int Port::evaluationOrder() const {
    return _evaluationOrder;
}

QVector<Port*> Port::importPorts() const {
    return _importPorts;
}

QVector<Port*> Port::exportPorts() const {
    return _exportPorts;
}

template <> Value Port::value() const {
    return _value;
}

void Port::verifyValue() const {
    switch (_value.type()) {
    case Value::Type::Double:
        if (std::fpclassify(_value.as<double>()) == FP_NAN)
            ThrowException("Value is not a number").context(this);
        break;
    case Value::Type::Date:
        if (!_value.as<QDate>().isValid())
            ThrowException("Date is invalid").context(this);
        break;
    case Value::Type::Time:
        if (!_value.as<QTime>().isValid())
            ThrowException("Time is invalid").context(this);
        break;
    case Value::Type::DateTime:
        if (!_value.as<QDateTime>().isValid())
            ThrowException("Date-time is invalid").context(this);
        break;
    default: ;
    }
}

const Expression& Port::expression() const {
    return _expression;
}

//
// Housekeeping
//

void Port::enumerate(int &number) {
    _evaluationOrder = number++;
}

void Port::closeExpression() {
    _expression.close();
}

void Port::addExportPort(Port *port) {
    if (!_exportPorts.contains(port))
        _exportPorts << port;
}

void Port::reset() {
    _value.reset();
}

void Port::update() {
    _value = _expression.evaluate();
}

// Access

Track::Order Port::track() {
    return Track::takeOrder(this);
}

void Port::toText(QTextStream &text, int indentation) const {
    QString fill;
    fill.fill(' ', indentation);
    QString prefix =
            (access() == PortAccess::Input) ?
            (_isExtraneous ? "+" : "." ) : "//~";
    bool showAmended = false;
    if (constructionStep() == ComputationStep::Amend) {
        Box *myBox = dynamic_cast<Box*>(parent());
        showAmended = (myBox && myBox->constructionStep() != ComputationStep::Amend);
    }
    QString comment = showAmended ? " //amended" : "";

    QString equalSign = (access() == PortAccess::Input) ? " = " : " == ";

    text << fill
         << prefix << objectName()
         << equalSign << _expression.originalAsString()
         << comment
         << "\n";
}

}

