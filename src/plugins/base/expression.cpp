/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <limits>
#include <sstream>
#include "exception.h"
#include "expression.h"
#include "convert_operator.h"
#include "convert.h"
#include "operate.h"
#include "value_collection.h"
using std::get;

namespace base {

Expression::Expression(Node *parent)
    : _parent(parent), _isClosed(false)
{
}

void Expression::setParent(Node *parent) {
    _parent = parent;
}

void Expression::clear() {
    _isClosed = false;
    _stack.clear();
    _original.clear();
}

//void Expression::translate(const boxscript::ast::Expression &e) {
//    clear();
//    push(e.firstOperand);
//    for (auto operation : e.operations) {
//        push(lookupOperator(QString::fromStdString(operation.operator_)));
//        push(operation.operand);
//    }
//    close();
//}

//void Expression::push(boxscript::ast::Operand operand) {
//    using Type = boxscript::ast::Operand::Type;
//    boxscript::ast::Number number;
//    switch(operand.type()) {
//    case Type::Bool:
//        push(boost::get<boxscript::ast::Bool>(operand));
//        break;
//    case Type::Number:
//        push(boost::get<boxscript::ast::Number>(operand));
//        break;
//    case Type::QuotedString:
//        push(boost::get<boxscript::ast::QuotedString>(operand));
//        break;
//    case Type::Date:
//        push(boost::get<boxscript::ast::Date>(operand));
//        break;
//    case Type::Time:
//        push(boost::get<boxscript::ast::Time>(operand));
//        break;
//    case Type::DateTime:
//        push(boost::get<boxscript::ast::DateTime>(operand));
//        break;
//    case Type::BareDate:
//        push(boost::get<boxscript::ast::BareDate>(operand));
//        break;
//    case Type::ReferenceUnion:
//        push(boost::get<boxscript::ast::ReferenceUnion>(operand));
//        break;
//    case Type::GroupedExpression:
//        push(boost::get<boxscript::ast::GroupedExpression>(operand));
//        break;
//    case Type::FunctionCall:
//        push(boost::get<boxscript::ast::FunctionCall>(operand));
//        break;
//    }
//}

//void Expression::push(boxscript::ast::Bool b) {
//    push(b.stringValue=="TRUE");
//}

//void Expression::push(boxscript::ast::Number number) {
//    if (number.type() == boxscript::ast::Number::Type::Int)
//        push(boost::get<int>(number));
//    else
//        push(boost::get<double>(number));
//}

//void Expression::push(boxscript::ast::QuotedString s) {
//    push(Value(QString::fromStdString(s.stringValue)));
//}

//void Expression::push(boxscript::ast::Date date) {
//    push(QDate(date.year, date.month, date.day));
//}

//void Expression::push(boxscript::ast::Time time) {
//    push(QTime(time.hour, time.minute, time.second));
//}

//void Expression::push(boxscript::ast::DateTime dt) {
//    push(Value(QDateTime(QDate(dt.date.year, dt.date.month, dt.date.day), QTime(dt.time.hour, dt.time.minute, dt.time.second), Qt::UTC)));
//}

//void Expression::push(boxscript::ast::BareDate bd) {
//    push(BareDate(bd.month, bd.day));
//}

//void Expression::push(boxscript::ast::Path path) {
////    std::stringstream str;
////    str << ref;
////    push(Path(QString::fromStdString(str.str())));
//}

//void Expression::push(boxscript::ast::GroupedExpression group) {
//    push(Parenthesis::Left);
//    const boxscript::ast::Expression &e(group.get());
//    translate(e); // push expression inside parentheses recursively
//    push(Parenthesis::Right);
//}

//void Expression::push(boxscript::ast::FunctionCall func) {
//    push(FunctionCall(QString::fromStdString(func.name), (int) func.arguments.size()));
//    for (boxscript::ast::FunctionCall::Argument arg : func.arguments)
//        push(arg.get());
//}

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

void Expression::push(FunctionEnd end) {
    checkNotClosed();
    _stack.push_back(end);
}

void Expression::checkNotClosed() {
    if (_isClosed)
        ThrowException("You cannot push to a closed stack").value(originalAsString()).context(_parent);
}

void Expression::close() {
    _original = _stack;
//    resolveImports();
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

inline bool isFunctionCall(Expression::Element element) {
    return (Expression::type(element) == Expression::Type::FunctionCall);
}

inline int precedence(Expression::Element element) {
    Q_ASSERT(isOperator(element) || isFunctionCall(element));
    return isOperator(element) ? precedence( get<Operator>(element) ) : 0;
}

//void Expression::resolveImports() {
//    for (auto element : _stack) {
//        if (isPath(element))
//            std::get<Path>(element).resolvePorts();
//    }
//}

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
            myStack.push_back(registerFunctionCall(element));
            break;
        case Type::FunctionEnd:
            // Pop until matching function call
            while (!(myStack.empty() || isFunctionCall(myStack.back()))) {
                postfix.push_back( myStack.back() );
                myStack.pop_back();
            }
            if (isFunctionCall(myStack.back())) {
                postfix.push_back( myStack.back() );
                myStack.pop_back();
            }
            else {
                ThrowException("Missing function call");
            }
            break;
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

Expression::Element Expression::registerFunctionCall(const Element &element) {
    FunctionCall f = std::get<FunctionCall>(element);
    f.id = _functionCalls.size();
    _functionCalls << f;
    return f;
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
    case Operator::Exponentiate : c = operate::exponentiate(a,b); break;
    case Operator::Larger       : c = operate::larger(a,b); break;
    case Operator::LargerOrEqual: c = operate::largerOrEqual(a,b); break;
    case Operator::Less         : c = operate::less(a,b); break;
    case Operator::LessOrEqual  : c = operate::lessOrEqual(a,b); break;
    case Operator::Equal        : c = operate::equal(a,b); break;
    case Operator::NotEqual     : c = operate::notEqual(a,b); break;
    case Operator::And          : c = operate::and_(a,b); break;
    case Operator::Or           : c = operate::or_(a,b); break;
    case Operator::Negate       : c = operate::negate(a); break;
    case Operator::Not          : c = operate::not_(a); break;
//    default                     : ThrowException("Unexpected operator").value(static_cast<int>(op));
    }
    // We need to pop operator and push result, because result may be of another type
    stack.pop_back();
    stack.push_back(c);
}

namespace {

Value::Type argumentsType(const Expression::Stack &stack, int arity) {
    QVector<const Value*> args;
    auto end = stack.cend(),
         begin = end - arity;
    for (auto it=begin; it<end; ++it) {
        args << & std::get<Value>(*it);
    }
    return ValueCollection::type(args);
}

template <class T> QVector<T> popArguments(Expression::Stack &stack, int arity) {
    QVector<T> args;
    for (int i=0; i<arity; ++i) {
        const Value &value = std::get<Value>(stack.back());
        if (value.isVector())
            args << value.as<QVector<T>>();
        else
            args << value.as<T>();
        stack.pop_back();
    }
    return args;
}

template <class T>
T sum(QVector<T> v) {
    T y = 0;
    for (auto x : v)
        y += x;
    return y;
}

template <class T>
T mean(QVector<T> v) {
    return (v.size() > 0) ? sum<T>(v)/v.size() : T();
}

template <class T>
T min(QVector<T> v) {
    if (v.isEmpty())
        ThrowException("Cannot find 'min' of empty vector");
    T y = v.first();
    for (auto x : v)
        if (x<y)
            y = x;
    return y;
}

template <class T>
T max(QVector<T> v) {
    if (v.isEmpty())
        ThrowException("Cannot find 'max' of empty vector");
    T y = v.first();
    for (auto x : v)
        if (x>y)
            y = x;
    return y;
}

bool any(QVector<bool> v) {
    for (auto x : v)
        if (convert<bool>(x))
            return true;
    return false;
}

bool all(QVector<bool> v) {
    for (auto x : v)
        if (!convert<bool>(x))
            return false;
    return true;
}

} // local namespace

void Expression::reduceByFunctionCall(Stack &stack) {
    // Pop operator
    const FunctionCall &func    = get<FunctionCall>(stack.back());
          FunctionCall &funcReg = _functionCalls[func.id];
    stack.pop_back();
    if ((int) stack.size() < func.arity)
        ThrowException("Wrong function arity").value(func.arity);

    using Type = Value::Type;
    if (funcReg.type == Value::Type::Uninitialized)
        funcReg.type = argumentsType(stack, func.arity);
    Type &type(funcReg.type);

    if (func.name == "sum") {
        switch (type) {
        case Type::Int   : stack.push_back(sum(popArguments<int   >(stack, func.arity))); break;
        case Type::Double: stack.push_back(sum(popArguments<double>(stack, func.arity))); break;
        default: ThrowException("Illegal argument type for 'sum'").value(Value::typeName(type));
        }
    }
    else if (func.name == "mean") {
        switch (type) {
        case Type::Int   : stack.push_back(mean(popArguments<int   >(stack, func.arity))); break;
        case Type::Double: stack.push_back(mean(popArguments<double>(stack, func.arity))); break;
        default: ThrowException("Illegal argument type for 'mean'").value(Value::typeName(type));
        }
    }
    else if (func.name == "min") {
        switch (type) {
        case Type::Int     : stack.push_back(min(popArguments<int      >(stack, func.arity))); break;
        case Type::Double  : stack.push_back(min(popArguments<double   >(stack, func.arity))); break;
        case Type::Date    : stack.push_back(min(popArguments<QDate    >(stack, func.arity))); break;
        case Type::Time    : stack.push_back(min(popArguments<QTime    >(stack, func.arity))); break;
        case Type::DateTime: stack.push_back(min(popArguments<QDateTime>(stack, func.arity))); break;
        case Type::BareDate: stack.push_back(min(popArguments<BareDate >(stack, func.arity))); break;
        default: ThrowException("Illegal argument type for 'min'").value(Value::typeName(type));
        }
    }
    else if (func.name == "max") {
        switch (type) {
        case Type::Int     : stack.push_back(max(popArguments<int      >(stack, func.arity))); break;
        case Type::Double  : stack.push_back(max(popArguments<double   >(stack, func.arity))); break;
        case Type::Date    : stack.push_back(max(popArguments<QDate    >(stack, func.arity))); break;
        case Type::Time    : stack.push_back(max(popArguments<QTime    >(stack, func.arity))); break;
        case Type::DateTime: stack.push_back(max(popArguments<QDateTime>(stack, func.arity))); break;
        case Type::BareDate: stack.push_back(max(popArguments<BareDate >(stack, func.arity))); break;
        default: ThrowException("Illegal argument type for 'max'").value(Value::typeName(type));
        }
    }
    else if (func.name == "any") {
        switch (type) {
        case Type::Bool  :
        case Type::Int   :
        case Type::Double: stack.push_back(any(popArguments<bool>(stack, func.arity))); break;
        default: ThrowException("Illegal argument type for 'any'").value(Value::typeName(type));
        }
    }
    else if (func.name == "all") {
        switch (type) {
        case Type::Bool  :
        case Type::Int   :
        case Type::Double: stack.push_back(all(popArguments<bool>(stack, func.arity))); break;
        default: ThrowException("Illegal argument type for 'all'").value(Value::typeName(type));
        }
    }
    else if (func.name == "c") {
        switch (type) {
        case Type::Bool    : stack.push_back(popArguments<bool     >(stack, func.arity)); break;
        case Type::Int     : stack.push_back(popArguments<int      >(stack, func.arity)); break;
        case Type::Double  : stack.push_back(popArguments<double   >(stack, func.arity)); break;
        case Type::String  : stack.push_back(popArguments<QString  >(stack, func.arity)); break;
        case Type::Date    : stack.push_back(popArguments<QDate    >(stack, func.arity)); break;
        case Type::Time    : stack.push_back(popArguments<QTime    >(stack, func.arity)); break;
        case Type::DateTime: stack.push_back(popArguments<QDateTime>(stack, func.arity)); break;
        case Type::BareDate: stack.push_back(popArguments<BareDate >(stack, func.arity)); break;
        default: ThrowException("Illegal argument type for 'c'").value(Value::typeName(type));
        }
    }
    else
        ThrowException("Unknown function").value(func.name);

    // More functions
    //  exists(path)
    // count(path)
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
            if (isOperator(element))
                reduceByOperator(myStack);
            else if (isFunctionCall(element))
                reduceByFunctionCall(myStack);
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

QString Expression::elementName(const Element& el) {
    using Type = Expression::Type;
    QString s;
    switch (type(el)) {
    case Type::Value        : s = "Value"       ; break;
    case Type::Operator     : s = "Operator"    ; break;
    case Type::Parenthesis  : s = "Parenthesis" ; break;
    case Type::Path         : s = "Path"        ; break;
    case Type::FunctionCall : s = "FunctionCall"; break;
    case Type::FunctionEnd  : s = "FunctionEnd" ; break;
    }
    return s;
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
    case Type::Value:       s = get<Value>(element).asString(true, true); break;
    case Type::Operator:    s = convert<QString>( get<Operator>   (element) ); break;
    case Type::Parenthesis: s = convert<QString>( get<Parenthesis>(element) ); break;
    case Type::Path:        s = get<Path>(element).toString(); break;
    case Type::FunctionCall:
        func = get<FunctionCall>(element);
        s = func.name + "[" + QString::number(func.arity) + "]";
        break;
    case Type::FunctionEnd:
        s = "#";
        break;
    }
    return s;
}

}
