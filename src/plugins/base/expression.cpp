/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <limits>
#include <sstream>
#include "box.h"
#include "convert_operator.h"
#include "convert.h"
#include "exception.h"
#include "expression.h"
#include "operate.h"
#include "phys_math.h"
#include "port.h"
#include "resolved_references.h"
#include "value_collection.h"

using std::get;

namespace base {

Expression::Expression(Port *parent)
    : _parent(parent), _isClosed(false), _isResolved(false)
{
}

void Expression::setParent(Port *parent) {
    _parent = parent;
    for (Element &element : _stack) {
        if (Expression::type(element) == Expression::Type::Path) {
            std::get<Path>(element).setParent(parent);
        }
    }
}

void Expression::clear() {
    _isClosed = _isResolved = false;
    _stack.clear();
    _original.clear();
}

bool Expression::isConstant() const {
    // The expression is constant is it is not empty and it contains no references
    // This can be improved to consider if the references referred to are also constant
    for (const Element &element : _stack) {
        if (type(element) == Type::ValuePtr || type(element) == Type::Path)
            return false;
    }
    return !_stack.empty();
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

inline bool isValuePtr(Expression::Element element) {
    return (Expression::type(element) == Expression::Type::ValuePtr);
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
        return precedence(Operator::Comma) - 1;
    case Expression::Type::Conditional:
        return precedence(Operator::Comma) - 2;
    case Expression::Type::Parenthesis:
        return precedence(Operator::Comma) - 3;
    default:
        ThrowException("Unknown precedence").value(Expression::typeName(element));
    }
}

void Expression::toPostfix() {
    // After: https://www.tutorialspoint.com/Convert-Infix-to-Postfix-Expression
    Stack postfix, myStack;
    for (auto &element : _stack) {
        switch (type(element)) {
        case Type::Value:
        case Type::ValuePtr:
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
            myStack.push_back(element);
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
    }
    // Copy result
    _stack.clear();
    _stack = postfix;
}

//Expression::Element Expression::registerFunctionCall(const Element &element) {
//    FunctionCall f = std::get<FunctionCall>(element);
//    f.id = _functionCalls.size();
//    _functionCalls << f;
//    return f;
//}

void Expression::reduceByOperator(Stack &stack) {
    // Pop operator
    Operator op = get<Operator>(stack.back());
    stack.pop_back();

    // Check stack size
    Q_ASSERT(static_cast<int>(stack.size()) >= arity((op)));

    // Pop second operand (it comes first from the stack!)
    const Value &b = isValue   (stack.back()) ?  get<Value   >(stack.back()) :
                     isValuePtr(stack.back()) ? *get<ValuePtr>(stack.back()) :
                     Value::null();
    stack.pop_back();

    // Pop first operand if present
    const Value &a = (arity(op)==2 && isValue   (stack.back())) ?  get<Value   >(stack.back()) :
                     (arity(op)==2 && isValuePtr(stack.back())) ? *get<ValuePtr>(stack.back()) :
                     Value::null();
    if (arity(op)==2)
        stack.pop_back();

    // Carry out the operation and leave result in stack top
    Value c;
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
    case Operator::Negate       : c = operate::negate(b); break;
    case Operator::Not          : c = operate::not_(b); break;
    case Operator::Comma        :ThrowException("Cannot reduce by comma operator").context(_parent); break;
    }
    stack.push_back(c);
}

namespace {

QVector<const Value*> findManyValues(const Path &path) {
    QVector<const Value*> values;
    for (Port *port : path.findMany<Port*>()) {
         values << &port->value();
    }
    return values;
}

Value::Type argumentsType(Expression::Stack &stack, int arity) {
    QVector<const Value*> args;
    auto end = stack.cend(),
         begin = end - arity;
    for (auto it=begin; it<end; ++it) {
        switch (Expression::type(*it)) {
        case Expression::Type::Value:
            args << const_cast<const Value*>(& std::get<Value>(*it));
            break;
        case Expression::Type::ValuePtr:
            args << std::get<Expression::ValuePtr>(*it);
            break;
        case Expression::Type::Path:
            args << findManyValues( std::get<Path>(*it) );
            break;
        default:
            ThrowException("Value or Path expected").value(Expression::typeName(*it));
        }
    }
    return ValueCollection::type(args);
}

template <class T> QVector<T> popArguments(Expression::Stack &stack, int arity) {
    QVector<T> args;
    int n = stack.size();
    for (int i=n-arity; i<n; ++i) {
        Q_ASSERT(isValue(stack.at(i)) || isValuePtr(stack.at(i)));
        const Value &value = isValue(stack.at(i)) ? std::get<Value>(stack.at(i)) : *std::get<Expression::ValuePtr>(stack.at(i));
        if (value.isVector())
            args << value.as<QVector<T>>(); // flatten vector
        else if (!value.isNull())
            args << value.as<T>();
    }
    stack.resize(n-arity);
    return args;
}

void  popArguments(Expression::Stack &stack, int arity) {
    int n = stack.size();
    stack.resize(n-arity);
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
    if (v.isEmpty())
        ThrowException("Cannot apply 'any' to an empty vector");
    for (auto x : v)
        if (convert<bool>(x))
            return true;
    return false;
}

bool all(QVector<bool> v) {
    if (v.isEmpty())
        ThrowException("Cannot apply 'all' to an empty vector");
    for (auto x : v)
        if (!convert<bool>(x))
            return false;
    return true;
}

inline bool isLikeNull(Value::Type type) {
    return type==Value::Type::Uninitialized || type==Value::Type::Null;

}

} // local namespace

void Expression::reduceByFunctionCall(Stack &stack) {
    // Pop function
    FunctionCall &func    = get<FunctionCall>(stack.back());
    stack.pop_back();
    if ((int) stack.size() < func.arity)
        ThrowException("Wrong function arity").value(func.arity);

    using Type = Value::Type;
    func.type = argumentsType(stack, func.arity);
    Type &type(func.type);

    // Functions that accepts null type
    if (func.name == "sum") {
        switch (type) {
        case Type::Null  : popArguments(stack, func.arity); stack.push_back(Value(int(0))); break;
        case Type::Int   : stack.push_back(sum(popArguments<int   >(stack, func.arity))); break;
        case Type::Double: stack.push_back(sum(popArguments<double>(stack, func.arity))); break;
        default: ThrowException("Illegal argument type for 'sum'").value(Value::typeName(type));
        }
    }
    else if (func.name == "mean") {
        switch (type) {
        case Type::Null  : popArguments(stack, func.arity); stack.push_back(Value(int(0))); break;
        case Type::Int   :
        case Type::Double: stack.push_back(mean(popArguments<double>(stack, func.arity))); break;
        default: ThrowException("Illegal argument type for 'mean'").value(Value::typeName(type));
        }
    }
    else if (func.name == "exists" || func.name == "count") {
        if (isLikeNull(type)) {
            popArguments(stack, func.arity);
            if (func.name == "exists")
                stack.push_back(Value(false));
            else
                stack.push_back(Value(int(0)));
        }
        else {
            int count;
            switch (type) {
            case Type::Bool    : count = popArguments<bool     >(stack, func.arity).size(); break;
            case Type::Int     : count = popArguments<int      >(stack, func.arity).size(); break;
            case Type::Double  : count = popArguments<double   >(stack, func.arity).size(); break;
            case Type::String  : count = popArguments<QString  >(stack, func.arity).size(); break;
            case Type::Date    : count = popArguments<QDate    >(stack, func.arity).size(); break;
            case Type::Time    : count = popArguments<QTime    >(stack, func.arity).size(); break;
            case Type::DateTime: count = popArguments<QDateTime>(stack, func.arity).size(); break;
            case Type::BareDate: count = popArguments<BareDate >(stack, func.arity).size(); break;
            default: ThrowException("Illegal argument type for "+func.name).value(Value::typeName(type));
            }
            if (func.name == "exists")
                stack.push_back(Value(bool(count)));
            else
                stack.push_back(Value(count));
        }
    }
    // Evaluate null to null
    else if (type == Type::Null) {
        stack.resize(stack.size() - func.arity);
        stack.push_back(Value::null());
    }
    // Functions that don't accept null type
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
        case Type::Null    : stack.push_back(Value::null()); break;
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
}

bool Expression::reduceByCondition(Stack &stack) {
    // Pop "If" or "Elsif" keyword
    stack.pop_back();
    // Pop boolean value
    Q_ASSERT(isValue(stack.back()) || isValuePtr(stack.back()));
    Value condition = isValue(stack.back()) ? get<Value>(stack.back()) : *get<ValuePtr>(stack.back());
    stack.pop_back();
    // Return whether condition was true
    return condition.as<bool>();
}

Value Expression::evaluate() {
    if (_stack.size() == 0)
        ThrowException("Expression stack is empty").context(_parent);

    // If references have not yet all been resolved
    // then save stack to restore it after evaluation
    Stack savedStack;
    bool keepStack = !ResolvedReferences::fixed();
    if (keepStack)
        savedStack = _stack;

    // If my references have not yet been resolved
    // then replace Path elements with Value elements
    if (!_isResolved)
        resolveReferences();

    // If all have been resolved
    // then keep the stack with all Path elements replaced by Value elements.
    // First time ::fixed() turns true _isResolved turns true and remains true.
    _isResolved = ResolvedReferences::fixed();

    enum class ConditionalPhase {FinishUponThen, SkipUntilThen, SkipUntilElse, Done};
    auto phase = ConditionalPhase::Done;

    Stack myStack;
    Element result;
    if (_stack.size() == 1) {
        result = _stack.front();
    }
    else {
        for (Element &element : _stack) {
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

    // Restore stack if references were not yet fixed
    if (keepStack) {
        _stack.clear();
        _stack = savedStack;
    }

    // Return result or null
    return isValue   (result) ?  get<Value>(result) :
           isValuePtr(result) ? *get<ValuePtr>(result) :
           Value::null();
}

Expression::Stack::iterator Expression::replaceElement(Stack::iterator at, const QVector<Port*> &ports) {
    at = _stack.insert(at, ports.size(), Element());
    for (auto port : ports)
        *at++ = port->valuePtr<Value>();
    *at++ = FunctionCall("c", ports.size());
    return at;
}

void Expression::resolveReferences() {
    _importPorts.clear();
    auto end0 = _stack.end(), end1=end0;
    auto element=_stack.begin();
    while (element!=_stack.end()) {
        if (type(*element) == Type::Path) {
            Path &path = get<Path>(*element);
            path.setParent(_parent);
            auto matches = path.findMany<Port*>();
            _importPorts.append(matches);
            ResolvedReferences::add(_parent, matches);

            switch (matches.size()) {
            case 0:
                *element = Value::null();
                ++element;
                break;
            case 1:
                *element = matches.at(0)->valuePtr<Value>();
                ++element;
                break;
            default:
                end0 = _stack.end();
                element = replaceElement(element, matches);
                end1 = _stack.end();
            }
        }
        else {
            ++element;
        }
    }
}

Port *Expression::parent() {
    return _parent;
}

const Expression::Stack& Expression::original() const {
    return _original;
}

const Expression::Stack& Expression::stack() const {
    return _stack;
}

QString Expression::typeName(const Element& el) {
    using Type = Expression::Type;
    QString s;
    switch (type(el)) {
    case Type::Value           : s = get<Value   >(el).typeName(); break;
    case Type::ValuePtr        : s = get<ValuePtr>(el)->typeName(); break;
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

const QVector<Port*> &Expression::importPorts() const {
    return _importPorts;
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
    case Type::Value:       s = get<Value   >(element). asString(); break;
    case Type::ValuePtr:    s = get<ValuePtr>(element)->asString(); break;
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
    return s +"{" + typeName(element) +"}";
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
