/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <limits>
#include <sstream>
#include "box.h"
#include "exception.h"
#include "expression.h"
#include "convert_operator.h"
#include "convert.h"
#include "operate.h"
#include "value_collection.h"
using std::get;

#include <iostream>

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

bool Expression::isFixed() const {
    for (auto element : _stack) {
        if (type(element) == Type::Path)
            return false;
    }
    return true;
}

void Expression::checkNotClosed() {
    if (_isClosed)
        ThrowException("You cannot push to a closed stack").value(originalAsString()).context(_parent);
}

void Expression::close() {
    _original = _stack;
    toPostfix();
    _isClosed = true;
}

inline bool isValue(Expression::Element element) {
    return (Expression::type(element) == Expression::Type::Value);
}

inline bool isOperator(Expression::Element element) {
    return (Expression::type(element) == Expression::Type::Operator);
}

inline bool isComma(Expression::Element element) {
    return isOperator(element) &&
           (get<Operator>(element) == Operator::Comma);
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

inline bool isConditional(Expression::Element element) {
    return (Expression::type(element) == Expression::Type::Conditional);
}

inline bool isConditionalThen(Expression::Element element) {
    return (isConditional(element) &&
            get<Expression::Conditional>(element) == Expression::Conditional::Then);
}

inline bool isConditionalElse(Expression::Element element) {
    return (isConditional(element) &&
            get<Expression::Conditional>(element) == Expression::Conditional::Else);
}

inline int precedence(Expression::Element element) {
    switch(Expression::type(element)) {
    case Expression::Type::Operator:
        return precedence( get<Operator>(element) );
    case Expression::Type::FunctionCall:
        return precedence(Operator::Comma)- 1;
    case Expression::Type::Conditional:
        return precedence(Operator::Comma)- 2;
    default:
        ThrowException("Unexpected element").value(Expression::elementName(element));
    }
}

void Expression::toPostfix() {
    QString s1 = toString(_stack);
    std::cout << "toPostfix " << qPrintable(s1) << std::endl;
    // After: https://www.tutorialspoint.com/Convert-Infix-to-Postfix-Expression
    Stack postfix, myStack;
    for (auto &element : _stack) {
        QString s1 = elementName(element),
                s2 = toString(postfix),
                s3 = toString(myStack);
        std::cout << qPrintable(s1) << " =>"
                  << "\npostfix stack = " << qPrintable(s2)
                  << "\n      mystack = " << qPrintable(s3)
                  << std::endl;
        switch (type(element)) {
        case Type::Value:
            postfix.push_back(element);
            break;
        case Type::Operator:
        case Type::Conditional:
            if (myStack.empty() || precedence(element) > precedence(myStack.back())) {
                // Comma goes on to postfix, other operator is kept for later
                if (isComma(element))
                    postfix.push_back(element);
                else
                    myStack.push_back(element);
            }
            else {
                // Pop until a higher precedence is found or stack is empty
                while (!myStack.empty() && precedence(element) <= precedence(myStack.back())) {
                    postfix.push_back( myStack.back() );
                    myStack.pop_back();
                    s2 = toString(postfix);
                    s3 = toString(myStack);
                    std::cout << "Push and pop"
                              << "\npostfix stack = " << qPrintable(s2)
                              << "\n      mystack = " << qPrintable(s3)
                              << std::endl;
                }
                // Comma goes on to postfix, other operator is kept for later
                if (isComma(element))
                    postfix.push_back(element);
                else
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
        case Type::FunctionCallEnd:
            // Push function call end on my stack
            // Note: A function call end is never transferred to the postfix stack
            postfix.push_back( myStack.back() );
            myStack.pop_back();
            break;
        }
    } // for
    // Pop until stack is empty
    while (!myStack.empty()) {
        postfix.push_back( myStack.back() );
        myStack.pop_back();
        QString s2 = toString(postfix),
                s3 = toString(myStack);
        std::cout << "Push and pop"
                  << "\npostfix stack = " << qPrintable(s2)
                  << "\n      mystack = " << qPrintable(s3)
                  << std::endl;
    }
    // Copy result
    _stack.clear();
    _stack = postfix;
    QString s2 = toString(_stack);
    std::cout << "toPostfix " << qPrintable(s2) << std::endl;
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
    bool bIsOk = true;
    // Pop operator
    Operator op = get<Operator>(stack.back());
    stack.pop_back();
    // Check stack sanity before popping second operand
    Q_ASSERT(static_cast<int>(stack.size()) >= arity((op)));
    Q_ASSERT(isValue(stack.back()));
    if (arity(op) == 2) {
        b = get<Value>(stack.back());
        bIsOk = (b.type() != Value::Type::Uninitialized);
        stack.pop_back();
    }
    // Check stack sanity before referencing stack top
    Q_ASSERT(isValue(stack.back()));
    // First operator
    const Value &a = get<Value>(stack.back());
    // Result
    Value c;
    // If operand(s) are OK then carry out operation and leave result in stack top
    if (a.type() != Value::Type::Uninitialized && bIsOk) {
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
        case Operator::Comma        :ThrowException("Cannot reduce by comma operator").context(_parent); break;
        }
    }
    // We need to pop operand and push result, because result may be of another type
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
    int n = stack.size();
    for (int i=n-arity; i<n; ++i) {
        const Value &value = std::get<Value>(stack.at(i));
        if (value.isVector())
            args << value.as<QVector<T>>(); // flatten vector
        else
            args << value.as<T>();
    }
    stack.resize(n-arity);
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

bool Expression::reduceByCondition(Stack &stack) {
    // Pop "If" or "Elsif" keyword
    stack.pop_back();
    // Pop boolean value
    Q_ASSERT(type(stack.back()) == Type::Value);
    Value condition = get<Value>(stack.back());
    stack.pop_back();
    // Return whether condition was true
    return condition.as<bool>();
}

Value Expression::evaluate() {
    if (_stack.size() == 0)
        ThrowException("Expression stack is empty").context(_parent);

    QString s1 = toString(_stack),
            s2, s3;
    std::cout << qPrintable(s1) << "=>\n"
              << "evaluate stack =\n" << qPrintable(s1)
              << std::endl;

    enum class ConditionalPhase {FinishUponThen, SkipUntilThen, SkipUntilElse, Done};
    auto phase = ConditionalPhase::Done;

    Stack myStack;
    Element result;
    if (_stack.size() == 1) {
        result = _stack.front();
    }
    else {
        for (Element &element : _stack) {
            s2 = toString(element);
            s3 = toString(myStack);
            std::cout << qPrintable(s2) << "=>\n"
                      << "mystack =\n" << qPrintable(s3)
                      << std::endl;

            if (phase == ConditionalPhase::SkipUntilThen) {
                if (isConditionalThen(element))
                    phase = ConditionalPhase::Done;
            }
            else if (phase == ConditionalPhase::SkipUntilElse) {
                if (isConditionalElse(element))
                    phase = ConditionalPhase::Done;
            }
            else if (isComma(element))
                ; // nothing to do
            else {
                myStack.push_back(element);
                if (isOperator(element))
                    reduceByOperator(myStack);
                else if (isFunctionCall(element))
                    reduceByFunctionCall(myStack);
                else if (isConditional(element)) {
                    switch (get<Conditional>(element)) {
                    case Conditional::If:
                    case Conditional::Elsif:
                        phase = reduceByCondition(myStack) ?
                                ConditionalPhase::FinishUponThen :
                                ConditionalPhase::SkipUntilThen;

                        break;
                    case Conditional::Then:
                        phase = ConditionalPhase::SkipUntilElse;
                        myStack.pop_back(); // pop 'Then'
                        break;
                    case Conditional::Else:
                        myStack.pop_back(); // pop 'Else'
                        break;
                    }
                }
            }
        }
        // The result should be the one element left in the stack
        if (myStack.size() == 0)
            ThrowException("Too many operators in expression").value(stackAsString()).context(_parent);
        if (myStack.size() > 1)
            ThrowException("Too few operators in expression").value(stackAsString()).context(_parent);
        result = myStack.front();
    }

    // The result should be a Value
    if (type(result) != Type::Value)
        ThrowException("Unexpected results type; expected a Value").value(toString(result)).context(_parent);
    Value value = get<Value>(result);
    return get<Value>(result);

}

Expression::Stack::iterator Expression::replaceElement(Stack::iterator at, const QVector<Port*> &ports) {
    QString s1 = stackAsString();
    at = _stack.insert(at, ports.size(), Element());
    QString s2 = stackAsString(),
            s3 = toString(*at);
    s3 = toString(*at);
    for (auto port : ports) {
        *at++ = port->value();
        s3 = stackAsString();
    }
    *at++ = FunctionCall("c", ports.size());
    s3 = stackAsString();
    return at;
}

bool Expression::resolveImports(Success rule) {
    bool allResolved = true;
    Box *box = boxAncestor();
    QString s1, s2;
    for (auto element=_stack.begin(); element!=_stack.end(); ++element) {
        s1 = stackAsString();
        s2 = toString(*element);
        if (type(*element) == Type::Path) {
            Path &path = get<Path>(*element);
            path.setParent(box);
            auto matches = path.findMany<Port*>();
            switch (matches.size()) {
            case 0:
                if (rule == Success::MustSucceed) {
                    ThrowException("Could not find port").
                            value1(originalAsString()).value2(path.toString()).
                            context(box);
                }
                allResolved = false;
                break;
            case 1:
                *element = matches.at(0)->value();
                break;
            default:
                element = replaceElement(element, matches);
            }
        }
    }
    s2 = stackAsString();
    std::cout << "Expression::resolveImports\n"
              << qPrintable(s1) << "\n"
              << qPrintable(s2) << std::endl;
    return allResolved;
}

Box *Expression::boxAncestor() {
    if (!_parent)
        ThrowException("Expressions needs a parent").value(originalAsString());
    auto *box  = dynamic_cast<Box* >(_parent);
    auto *port = dynamic_cast<Port*>(_parent);
    if (port)
        box = port->parent<Box*>();
    if (!box)
        ThrowException("Expression must be declated inside a box").value(originalAsString());
    return box;
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
    case Type::Value           : s = "Value"          ; break;
    case Type::Operator        : s = "Operator"       ; break;
    case Type::Parenthesis     : s = "Parenthesis"    ; break;
    case Type::Path            : s = "Path"           ; break;
    case Type::FunctionCall    : s = "FunctionCall"   ; break;
    case Type::FunctionCallEnd : s = "FunctionCallEnd"; break;
    case Type::Conditional     : s = "Conditional"    ; break;
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
    case Type::FunctionCallEnd: s = "end"; break;
    case Type::Conditional:     s = conditionalToString(get<Conditional>(element)); break;
    }
    return s;
}

QString conditionalToString(Expression::Conditional cond) {
    static QMap<Expression::Conditional, QString> map =
    {
        {Expression::Conditional::If,    "if"},
        {Expression::Conditional::Then,  "then"},
        {Expression::Conditional::Else,  "else"},
        {Expression::Conditional::Elsif, "elsif"}
    };
    return map.value(cond);
}

}
