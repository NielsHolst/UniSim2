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
#include "operator.h"
#include "path.h"
#include "success.h"
#include "value.h"

namespace base {

class Box;

class Expression
{
public:
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
    using Element = std::variant<
        Value,
        Operator,
        Parenthesis,
        Path,
        FunctionCall,
        FunctionCallEnd
    >;
    enum class Type {
        Value,
        Operator,
        Parenthesis,
        Path,
        FunctionCall,
        FunctionCallEnd
    };
    using Stack = std::vector<Element>;

    Expression(Node *parent=nullptr);
    void setParent(Node *parent);
    void clear();
    int size() const     { return _stack.size();}
    bool isEmpty() const { return _stack.empty();}
    bool isFixed() const;
    void push(Value value);
    void push(Operator operatr);
    void push(Parenthesis parenthesis);
    void push(Path path);
    void push(FunctionCall func);
    void push(FunctionCallEnd end);
    void close();
    bool resolveImports(Success rule);
    Value evaluate();

    Box *boxAncestor();
    const Stack& original() const;
    const Stack& stack() const;
    const Element& at(int i) const { return _stack.at(i); }
    Type type(int i) { return type(at(i)); }
    static Type type(const Element& el) { int i=el.index(); return static_cast<Type>(i); }
    static QString elementName(const Element& el);

    QString originalAsString() const;
    QString stackAsString() const;

private:
    // Data
    Node *_parent;
    Stack _stack, _original;
    bool _isClosed;
    QVector<FunctionCall> _functionCalls;
    // Methods
    void toPostfix();
    void checkNotClosed();
    Element registerFunctionCall(const Element &element);
    void reduceByOperator(Stack &stack);
    void reduceByFunctionCall(Stack &stack);
    Stack::iterator replaceElement(Stack::iterator at, const QVector<Port*> &ports);
    static QString toString(const Stack &stack);
    static QString toString(const Element &element);
};

}
#endif
