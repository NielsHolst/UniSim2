/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <sstream>
#include "exception.h"
#include "expression.h"
#include "convert_operator.h"
#include "convert.h"
#include "operate.h"
using std::get;

namespace base {

Expression::Expression(QObject *parent)
    : _parent(parent), _isClosed(false)
{
}

void Expression::clear() {
    _isClosed = false;
    _stack.clear();
    _original.clear();
}

void Expression::translate(const boxscript::ast::Expression &e) {
    clear();
    push(e.firstOperand);
    for (auto operation : e.operations) {
        push(lookupOperator(QString::fromStdString(operation.operator_)));
        push(operation.operand);
    }
    close();
}

void Expression::push(boxscript::ast::Operand operand) {
    using Type = boxscript::ast::Operand::Type;
    boxscript::ast::Number number;
    switch(operand.type()) {
    case Type::Bool:
        push(boost::get<boxscript::ast::Bool>(operand));
        break;
    case Type::Number:
        push(boost::get<boxscript::ast::Number>(operand));
        break;
    case Type::QuotedString:
        push(boost::get<boxscript::ast::QuotedString>(operand));
        break;
    case Type::Date:
        push(boost::get<boxscript::ast::Date>(operand));
        break;
    case Type::Time:
        push(boost::get<boxscript::ast::Time>(operand));
        break;
    case Type::DateTime:
        push(boost::get<boxscript::ast::DateTime>(operand));
        break;
    case Type::BareDate:
        push(boost::get<boxscript::ast::BareDate>(operand));
        break;
    case Type::Reference:
        push(boost::get<boxscript::ast::Reference>(operand));
        break;
    case Type::GroupedExpression:
        push(boost::get<boxscript::ast::GroupedExpression>(operand));
        break;
    case Type::FunctionCall:
        push(boost::get<boxscript::ast::FunctionCall>(operand));
        break;
    }
}

void Expression::push(boxscript::ast::Bool b) {
    push(b.stringValue=="TRUE");
}

void Expression::push(boxscript::ast::Number number) {
    if (number.type() == boxscript::ast::Number::Type::Int)
        push(boost::get<int>(number));
    else
        push(boost::get<double>(number));
}

void Expression::push(boxscript::ast::QuotedString s) {
    push(Value(QString::fromStdString(s.stringValue)));
}

void Expression::push(boxscript::ast::Date date) {
    push(QDate(date.year, date.month, date.day));
}

void Expression::push(boxscript::ast::Time time) {
    push(QTime(time.hour, time.minute, time.second));
}

void Expression::push(boxscript::ast::DateTime dt) {
    push(Value(QDateTime(QDate(dt.date.year, dt.date.month, dt.date.day), QTime(dt.time.hour, dt.time.minute, dt.time.second), Qt::UTC)));
}

void Expression::push(boxscript::ast::BareDate bd) {
    push(BareDate(bd.month, bd.day));
}

void Expression::push(boxscript::ast::Reference ref) {
    std::string s = ref.path + '[' + ref.port + ']';
    push(Path(QString::fromStdString(s)));
}

void Expression::push(boxscript::ast::GroupedExpression group) {
    push(Parenthesis::Left);
    const boxscript::ast::Expression &e(group.get());
    translate(e); // push expression inside parentheses recursively
    push(Parenthesis::Right);
}

void Expression::push(boxscript::ast::FunctionCall func) {
    push(FunctionCall{QString::fromStdString(func.name), (int) func.arguments.size()});
    for (boxscript::ast::FunctionCall::Argument arg : func.arguments)
        push(arg.get());
}

void Expression::push(Value value) {
    checkNotClosed();
    _stack.push_back(value);
}

void Expression::push(Operator operatr) {
    checkNotClosed();
    _stack.push_back(operatr);
}

void Expression::push(Parenthesis parenthesis) {
    checkNotClosed();
    _stack.push_back(parenthesis);
}

void Expression::push(Path path) {
    checkNotClosed();
    _stack.push_back(path);
}

void Expression::push(FunctionCall func) {
    checkNotClosed();
    _stack.push_back(func);
}

void Expression::push(Comma comma) {
    checkNotClosed();
    _stack.push_back(comma);
}

void Expression::checkNotClosed() {
    if (_isClosed)
        ThrowException("You cannot push to a closed stack").value(originalAsString()).context(_parent);
}

void Expression::close() {
    _original = _stack;
    resolvePaths();
    toPostfix();
    _isClosed = true;
}

inline bool isValue(Expression::Element element) {
    return (Expression::type(element) == Expression::Type::Value);
}

inline bool isOperator(Expression::Element element) {
    return (Expression::type(element) == Expression::Type::Operator);
}

inline bool isParenthesis(Expression::Element element) {
    return (Expression::type(element) == Expression::Type::Parenthesis);
}

inline bool isPath(Expression::Element element) {
    return (Expression::type(element) == Expression::Type::Path);
}

inline bool isLeft(Expression::Element element) {
    return isParenthesis(element) &&
           (get<Parenthesis>(element) == Parenthesis::Left);
}

inline bool isRight(Expression::Element element) {
    return isParenthesis(element) &&
           (get<Parenthesis>(element) == Parenthesis::Right);
}

inline bool isComma(Expression::Element element) {
    return (Expression::type(element) == Expression::Type::Comma);
}

inline int precedence(Expression::Element element) {
    Q_ASSERT(isOperator(element));
    return precedence( get<Operator>(element) );
}

void Expression::resolvePaths() {

}

void Expression::collectValues() {
    Stack myStack;
    for (auto &element : _stack) {
        if (type(element) == Type::FunctionCall) {

        }
    }
}

void Expression::toPostfix() {
    // From: https://www.tutorialspoint.com/Convert-Infix-to-Postfix-Expression
    Stack postfix, myStack;
    for (auto &element : _stack) {
        switch (type(element)) {
        case Type::Value:
            postfix.push_back(element);
            break;
        case Type::Operator:
            if (myStack.empty() || precedence(element) > precedence(myStack.back()))
                myStack.push_back(element);
            else {
                // Pop until a higher precedence is found
                while (!myStack.empty() && precedence(element) <= precedence(myStack.back())) {
                    if (!isComma(myStack.back()))
                        postfix.push_back( myStack.back() );
                    myStack.pop_back();
                }
                myStack.push_back(element);
            }
            break;
        case Type::Parenthesis:
            if (isLeft(element))
                myStack.push_back(element);
            else {
                // Pop until matching left parenthesis
                while (!(myStack.empty() || isLeft(myStack.back()))) {
                    postfix.push_back( myStack.back() );
                    myStack.pop_back();
                }
                if (myStack.empty())
                    ThrowException("No matching left parenthesis").context(_parent);
                myStack.pop_back(); // pop left parenthesis
                if (!myStack.empty() && type(myStack.back())==Type::FunctionCall) {
                    postfix.push_back( myStack.back() );
                    myStack.pop_back();
                }
            }
            break;
        case Type::Path:
            postfix.push_back(element);
            break;
        case Type::FunctionCall:
            myStack.push_back(element);
            break;
        case Type::Comma:
            while (!(myStack.empty() || isLeft(myStack.back()) || isComma(myStack.back()))) {
                postfix.push_back( myStack.back() );
                myStack.pop_back();
            }
            break;
        }
    } // for
    // Pop until stack is empty
    while (!myStack.empty()) {
        if (!isComma(myStack.back()))
            postfix.push_back( myStack.back() );
        myStack.pop_back();
    }
    // Copy result
    _stack.clear();
    _stack = postfix;
}

void Expression::reduceByOperator(Stack &stack) {
    // Second operator for arity==2
    Value b;
    // Pop operator
    Operator op = get<Operator>(stack.back());
    stack.pop_back();
    // Check stack sanity before popping second operand
    Q_ASSERT(static_cast<int>(stack.size()) >= arity((op)));
    Q_ASSERT(isValue(stack.back()));
    if (arity(op) == 2) {
        b = get<Value>(stack.back());
        stack.pop_back();
    }
    // Check stack sanity before referencing stack top
    Q_ASSERT(isValue(stack.back()));
    // First operator
    const Value &a = get<Value>(stack.back());
    // Result
    Value c;
    // Carry out operation and leave result in stack top
    switch (op) {
    case Operator::Add          : c = operate::add(a,b); break;
    case Operator::Subtract     : c = operate::subtract(a,b); break;
    case Operator::Multiply     : c = operate::multiply(a,b); break;
    case Operator::Divide       : c = operate::divide(a,b); break;
    case Operator::Negate       : c = operate::negate(a); break;
    case Operator::Exponentiate : c = operate::exponentiate(a,b); break;
    case Operator::And          : c = operate::and_(a,b); break;
    case Operator::Or           : c = operate::or_(a,b); break;
    case Operator::Not          : c = operate::not_(a); break;
    }
    // We need to pop operator and push result, because result may be of another type
    stack.pop_back();
    stack.push_back(c);
}

void Expression::reduceByFunctionCall(Stack &stack) {
    // Pop operator
    const FunctionCall &func = get<FunctionCall>(stack.back());
    stack.pop_back();
}

Value Expression::evaluate() {
    if (_stack.size() == 0)
        ThrowException("Expression is empty").context(_parent);

    Stack myStack;
    Element result;
    if (_stack.size() == 1) {
        result = _stack.front();
    }
    else {
        for (Element &element : _stack) {
            myStack.push_back(element);
            if (Expression::type(element) == Expression::Type::Operator)
                reduceByOperator(myStack);
        }
        // The result should be the one element left in the stack
        if (myStack.size() == 0)
            ThrowException("Too many operators in expression").value(stackAsString()).context(_parent);
        if (myStack.size() > 1)
            ThrowException("Too few operators in expression").value(stackAsString()).context(_parent);
        result = myStack.front();
    }

    // The result should be a Value
    if (type(result) == Type::Operator)
        ThrowException("Surplus operator in expression").value(stackAsString()).context(_parent);
    if (type(result) == Type::Parenthesis)
        ThrowException("Surplus parenthesis in expression").value(stackAsString()).context(_parent);
    Value value = get<Value>(result);
    return get<Value>(result);

}

const Expression::Stack& Expression::original() const {
    return _original;
}

const Expression::Stack& Expression::stack() const {
    return _stack;
}

QString Expression::originalAsString() const {
    return toString(_isClosed ? original() : stack());
}

QString Expression::stackAsString() const {
    return toString(stack());
}

QString Expression::toString(const Stack &stack) {
    QStringList str;
    for (auto &element : stack)
        str << toString(element);
    return str.join(" ");
}

QString Expression::toString(const Element &element) {
    Expression::FunctionCall func;
    QString s;
    switch (type(element)) {
    case Type::Value: s = get<Value>(element).asString(true, true); break;
    case Type::Operator: s = convert<QString>( get<Operator>   (element) ); break;
    case Type::Parenthesis: s = convert<QString>( get<Parenthesis>(element) ); break;
    case Type::Path: s = get<Path>(element).original(); break;
    case Type::FunctionCall:
            func = get<FunctionCall>(element);
            s = func.name + "[" + QString::number(func.arity) + "]";
            break;
    case Type::Comma: s = ", "; break;
    }
    return s;
}

}
