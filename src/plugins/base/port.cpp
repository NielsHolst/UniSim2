/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "boxscript_parser.h"
#include "path.h"
#include "port.h"

#include <iostream>

namespace base {

Port::Port(QString name, QObject *parent)
    : Node(name, parent),
      _isValueOverridden(false),
      _doReset(true),
      _isAuxilliary(false),
      _access(PortAccess::Input),
      _expression(this)
{
    QString s = parent ? Node::fullName(parent) : "No parent";
    std::cout << "Create port " << qPrintable(s+"["+name+"]") << std::endl;
    Box *boxParent = dynamic_cast<Box*>(parent);
    if (boxParent)
        boxParent->addPort(this);
}

void Port::assign(const Port &x) {
   _isValueOverridden = x._isValueOverridden;
   _doReset = x._doReset;
   _isAuxilliary = x._isAuxilliary;
   _access = x._access;
   _unit = x._unit;
   _help = x._help;
   _value = x._value;
   _expression = x._expression;
}

void Port::checkIfValueOverridden() {
    Box *boxParent = dynamic_cast<Box*>(parent());
    _isValueOverridden =  boxParent && !boxParent->underConstruction();
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

Port& Port::isAuxilliary() {
    _isAuxilliary = true;
    return *this;
}

void Port::outputNames(QStringList columnNames) {
    _outputNames = columnNames;
}

Port& Port::equals(const Expression &expression) {
    checkIfValueOverridden();
    _expression = expression;
    _expression.setParent(this);
    return *this;
}

Port& Port::imports(QString pathToPort, Caller caller) {
    checkIfValueOverridden();
    _expression.push(Path(pathToPort, this));
    _expression.setParent(this);
    _importCaller = caller;
    return help("Defaults to " + pathToPort);
}

Port& Port::computes(QString expression) {
    equals( boxscript::parser::parseExpression(expression) );
    return *this;
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

PortAccess Port::access() const {
    return _access;
}

QString Port::unit() const {
    return _unit;
}

QString Port::help() const {
    return _help;
}

QString Port::importPath() const {
    if (_expression.size() != 1) {
        ThrowException("Port expression must have only on token").
            value(_expression.originalAsString()).context(this);
    }

    const Expression::Element &element(_expression.original().at(0));
    if (Expression::type(element) != Expression::Type::Path) {
        ThrowException("Port expression must be a path").
            value(_expression.originalAsString()).context(this);
    }

    const Path &path(std::get<Path>(element));
    return path.toString();
}

int Port::size() const {
    return _value.size();
}

bool Port::isValueOverridden() const {
    return _isValueOverridden;
}

QStringList Port::outputNames() const {
    return _outputNames;
}

QVector<Port*> Port::importPorts() const {
    return _importPorts;
}

QVector<Port*> Port::exportPorts() const {
    return _exportPorts;
}

const Value& Port::value() const {
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

QString Port::format() const {
    using Type = Value::Type;
    switch (_value.baseType()) {
        case Type::Date:
            return "ymd";
            break;
        case Type::Time:
            return "HMS";
            break;
        case Type::DateTime:
            return "ymdHMS";
            break;
        default:
            return "NA";
    }
}

//
// Housekeeping
//

//void Port::resolveImports() {
//    _expression.resolveImports();
//}

void Port::addExportPort(Port *port) {
    if (!_exportPorts.contains(port))
        _exportPorts << port;
}

void Port::reset() {
    if (_doReset)
        _value.reset();
}

void Port::update() {
    _value = _expression.evaluate();
}

// Access

void Port::toText(QTextStream &text, int indentation) {
    update(); // make certain that _value is initialized
    QString fill, expression;
    fill.fill(' ', indentation);
    QString prefix =
            (access() == PortAccess::Input) ?
            (_isAuxilliary ? "+" : "." ) : "//~";

    text << fill
         << prefix << objectName()
         << " = " << _expression.originalAsString()
         << "//" + _value.typeName()
         << "\n";
}

}

