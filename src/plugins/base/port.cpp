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
      _isFixed(false),
      _expression(this)
{
    Box *boxParent = dynamic_cast<Box*>(parent);
    if (boxParent)
        boxParent->addPort(this);
    _clearAtReset = (type == Type::Output);
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
    _value = value; // value may be undefined but is used to give _value the right type
    _value.clear(); // _value is set to T()
    return *this;
}

void Port::define() {
    // Check parent
    auto *box = parent<Box*>();
    if (!box)
        ThrowException("Port is missing a parent box").context(this);

    // Check that (re-) definition is legal in this step
    auto step = Computation::currentStep();
    if (step >= Computation::Step::Initialize)
        ThrowException("Change of port definition only allowed until amend step").
                value(Computation::toString(step)).context(this);

    // Register re-definition
    _hasBeenRedefined = (step > Computation::Step::Construct);

    // Set me as parent of expression
    _expression.setParent(this);

    // Evaluation may not complete but try to evaluate value
    touch();
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

void Port::touch() {
    // Tentative evaluation. Imports of yet-undefined ports in _expression
    // will be replaced ny null values; hence _expression is kept
    auto keep = _expression;
    evaluate();
    _expression = keep;
}

void Port::evaluate() {
    QString s1 = _expression.originalAsString(),
            s2 = _expression.stackAsString(),
            s3 = _value.asString(true, true) + "{"+_value.typeName()+"}" ;
    if (s1.startsWith("if"))
        std::cout << qPrintable(name()+" evaluate A\n"+s1+"\n"+s2+"\n"+s3+"\n") <<
                     "fixed=" <<_isFixed << " resolved=" << Expression::allReferencesHaveBeenResolved() << std::endl;

    // A fixed port needs no evaluation
    if (_isFixed)
        return;

    // A Path value is set equal to the expression, presumable a valid path
    if (_value.type() == Value::Type::Path) {
        _value = Path(&_expression);
        return;
    }


    if (!_expression.isEmpty()) {
        if (_type == Type::Auxiliary &&
            (Computation::currentStep() <= Computation::Step::Reset ||
             _value.type() == Value::Type::Uninitialized ||
             !Expression::allReferencesHaveBeenResolved()))
            // Overwrite value's type, as an auxillary port may change type, since its expression
            // is evaluated again and again and additional references may have been resolved
            _value.overwrite(_expression.evaluate());
        else
            // Update value, keeping its type, which for inputs and outputs were defined
            // in the C++ code; the expression's type will be converted to the value's types
            // For aux ports, all references have now been resolved and the value's type thereby fixed
            _value = _expression.evaluate();
    }
    s1 = _expression.originalAsString(),
    s2 = _expression.stackAsString(),
    s3 = "Value = " + _value.asString(true, true) + "{"+_value.typeName()+"}" ;
    if (s1.startsWith("if"))
        std::cout << qPrintable(name()+" evaluate Z\n"+s1+"\n"+s2+"\n"+s3+"\n") << std::endl;

    // Register if the value will remain fixed after reset
    if (Computation::currentStep() == Computation::Step::Reset)
        _isFixed = _expression.isFixed();

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
    QString fill, expression;
    fill.fill(' ', indentation);
    text << fill
         << prefix.value(_type) << objectName()
         << " = " << _expression.originalAsString()
         << "//" << _value.typeName()
         << "//" << order()
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

