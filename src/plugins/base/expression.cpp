/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
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
    return (element.index() == 0);
}

inline bool isOperator(Expression::Element element) {
    return (element.index() == 1);
}

inline bool isParenthesis(Expression::Element element) {
    return (element.index() == 2);
}

inline bool isPath(Expression::Element element) {
    return (element.index() == 3);
}

inline int precedence(Expression::Element element) {
    Q_ASSERT(isOperator(element));
    return precedence( get<Operator>(element) );
}

inline bool isLeft(Expression::Element element) {
    return isParenthesis(element) &&
           (get<Parenthesis>(element) == Parenthesis::Left);
}

inline bool isRight(Expression::Element element) {
    return isParenthesis(element) &&
           (get<Parenthesis>(element) == Parenthesis::Right);
}

void Expression::resolvePaths() {

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
                while (!(isLeft(myStack.front()))) {
                    postfix.push_back( myStack.back() );
                    myStack.pop_back();
                }
                myStack.pop_back(); // pop left parenthesis
                if (myStack.empty())
                    ThrowException("No matching left parenthesis").context(_parent);
            }
            break;
        case Type::Path:
            ThrowException("Unimplemented").value(originalAsString()).context(_parent);
        }
    } // for
    // Pop until stack is empty
    while (!myStack.empty()) {
        postfix.push_back( myStack.back() );
        myStack.pop_back();
    }
    // Copy result
    _stack.clear();
    _stack = postfix;
}

void Expression::reduce(Stack &stack) {
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
                reduce(myStack);
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
    QString s;
    switch (type(element)) {
    case Type::Value: s = get<Value>(element).asString(true, true); break;
    case Type::Operator: s = convert<QString>( get<Operator>   (element) ); break;
    case Type::Parenthesis: s = convert<QString>( get<Parenthesis>(element) ); break;
    case Type::Path: s = get<Path>(element).original();
    }
    return s;
}

}
