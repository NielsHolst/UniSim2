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

Port::Port(QString name, Type type, Node *parent)
    : Node(name, parent),
      _type(type),
      _hasBeenRedefined(false),
      _hasBeenFixed(false),
      _expression(this)
{
    Box *boxParent = dynamic_cast<Box*>(parent);
    if (boxParent)
        boxParent->addPort(this);
    _clearAtReset = (type == Type::Output);
    clear();
}

Port& Port::doClear() {
    _clearAtReset = true;
    return *this;
}

Port& Port::noClear() {
    _clearAtReset = false;
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

void Port::outputNames(QStringList columnNames) {
    _outputNames = columnNames;
}

//
// Define port
//

Port& Port::initialize(Value value) {
    _value = value;
    return *this;
}

void Port::define() {
    // Check parent
    auto *box = parent<Box*>();
    if (!box)
        ThrowException("Port is missing a parent box").context(this);

    // Check that (re-) definition is legal in this step
    auto step = box->computationStep();
    if (step != Box::ComputationStep::Construct && step != Box::ComputationStep::Amend)
        ThrowException("Change of port definition only allowed in constructor and amend function").context(this);

    // Register re-definition
    _hasBeenRedefined = (step != Box::ComputationStep::Construct);

    // Set me as parent of expression
    _expression.setParent(this);

    // Evaluate to update value (if possible)
    QString s1 = _value.asString(true, true);
    evaluate(Success::MaySucceed);
    QString s2 = _value.asString(true, true);
    std::cout << "Port::define()\n"
              << qPrintable(s1) << "\n"
              << qPrintable(s2) << std::endl;
}

Port& Port::equals(const Value &value) {
    _expression.clear();
    _expression.push(value);
    define();
    return *this;
}

Port& Port::equals(const Expression &expression) {
    _expression = expression;
    define();
    return *this;
}

Port& Port::imports(QString pathToPort, Caller caller) {
    _importCaller = caller;
    _expression.push(Path(pathToPort, this));
    define();
    return help("Defaults to " + pathToPort);
}

Port& Port::computes(QString expression) {
    return equals( boxscript::parser::parseExpression(expression) );
}

//
// Change value
//

void Port::clear() {
    if (_clearAtReset)
        _value.clear();
}

void Port::evaluate(Success rule) {
    // Evaluate only if port has an expression
    // and when all imports have been resolved
    if (!_expression.isEmpty() && _expression.resolveImports(rule))
        _value = _expression.evaluate();
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

Port::Type Port::type() const {
    return _type;
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
    return _hasBeenRedefined;
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

void Port::addExportPort(Port *port) {
    if (!_exportPorts.contains(port))
        _exportPorts << port;
}

// Access

void Port::toText(QTextStream &text, int indentation) {
    static QMap<Type,QString> prefix =
    {{Type::Input, "."}, {Type::Output, "//."}, {Type::Input, "&"}};
    evaluate(Success::MaySucceed); // make certain that _value has been initialized
    QString fill, expression;
    fill.fill(' ', indentation);
    text << fill
         << prefix.value(_type) << objectName()
         << " = " << _expression.originalAsString()
         << "//" + _value.typeName()
         << "\n";
}

#define CASE_TYPE(x) case Port::Type::x: return #x

template<> QString convert(Port::Type type) {
    switch (type) {
        CASE_TYPE(Input);
        CASE_TYPE(Output);
        CASE_TYPE(Auxiliary);
    }
    return QString();
}

}

