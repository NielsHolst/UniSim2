/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_EXPRESSION_H
#define BASE_EXPRESSION_H
#include <stack>
#include <variant>
#include <vector>
#include "boxscript_ast.h"
#include "computation_step.h"
#include "operator.h"
#include "path.h"
#include "success.h"
#include "value.h"

namespace base {

class Box;
class Port;

class Expression
{
public:
    using ValuePtr = const Value*;
    struct FunctionCall {
        FunctionCall() {}
        FunctionCall(QString name_, int arity_) :
            name(name_), arity(arity_), type(Value::Type::Uninitialized), id(0) {}
        QString name;
        int arity;
        Value::Type type;
        int id;
    };
    struct FunctionCallEnd {
    };
    enum class Conditional {If, Then, Elsif, Else};
    using Element = std::variant<
        Value,
        ValuePtr,
        Operator,
        Parenthesis,
        Path,
        FunctionCall,
        FunctionCallEnd,
        Conditional
    >;
    enum class Type {
        Value,
        ValuePtr,
        Operator,
        Parenthesis,
        Path,
        FunctionCall,
        FunctionCallEnd,
        Conditional
    };
    using Stack = std::vector<Element>;

    Expression(Node *parent=nullptr);
    void setParent(Node *parent);
    void clear();
    int size() const     { return _stack.size();}
    bool isEmpty() const { return _stack.empty();}
    bool isFixed() const;
    template <class T> void push(T element);
    void close();
    void resolveReferences();
    Value evaluate();

    Box *boxAncestor();
    const Stack& original() const;
    const Stack& stack() const;
    const Element& at(int i) const { return _stack.at(i); }
    Type type(int i) { return type(at(i)); }
    static Type type(const Element& el) { int i=el.index(); return static_cast<Type>(i); }
    static QString typeName(const Element& el);

    QString originalAsString() const;
    QString stackAsString() const;

    static QString toString(const Stack &stack);
    static QString toString(const Element &element);

    struct ResolvedReference {
        const Node *referee;
        const Port *reference;
    };
    using ResolvedReferences = QHash<ResolvedReference, Computation::Step>;

    static void resetResolvedReferences();
    static int numResolvedReferences();
    static const ResolvedReferences &resolvedReferences();
    static void fixResolvedReferences();
    static bool allReferencesHaveBeenResolved();

private:
    // Data
    Node *_parent;
    Stack _stack, _original;
    bool _isClosed;
//    QVector<FunctionCall> _functionCalls;
    static ResolvedReferences _resolvedReferences;
    static bool _fixedResolvedReferences;

    // Methods
    void toPostfix();
    void checkNotClosed();
//    Element registerFunctionCall(const Element &element);
    void reduceByOperator(Stack &stack);
    void reduceByFunctionCall(Stack &stack);
    bool reduceByCondition(Stack &stack);
    Stack::iterator replaceElement(Stack::iterator at, const QVector<Port*> &ports);

    void addResolvedReferences(QVector<Port *> ports);
};

template <class T> void Expression::push(T element)
{     checkNotClosed();
      _stack.push_back(element);
}

size_t qHash(const Expression::ResolvedReference &key);
bool operator==(const Expression::ResolvedReference &a, const Expression::ResolvedReference &b);
std::ostream& operator<<(std::ostream& os, const Expression::ResolvedReference &ref);

QString conditionalToString(Expression::Conditional cond);

}
#endif
