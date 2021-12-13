/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_EXPRESSION_H
#define BASE_EXPRESSION_H
#include <stack>
#include <variant>
#include <vector>
#include <QObject>
#include "boxscript_ast.h"
#include "operator.h"
#include "path.h"
#include "value.h"

namespace base {

class Expression
{
public:
    struct FunctionCall {
        QString name;
        int arity;
    };
    struct Comma {
    };
    using Element = std::variant<
        Value,
        Operator,
        Parenthesis,
        Path,
        FunctionCall,
        Comma
    >;
    enum class Type {
        Value,
        Operator,
        Parenthesis,
        Path,
        FunctionCall,
        Comma
    };
    using Stack = std::vector<Element>;

    Expression(QObject *parent=nullptr);
    void clear();
    int size() const     { return _stack.size();}
    bool isEmpty() const { return _stack.empty();}
    void translate(const boxscript::ast::Expression &astExpression);
    void push(boxscript::ast::Operand operand);
    void push(boxscript::ast::Bool b);
    void push(boxscript::ast::Number number);
    void push(boxscript::ast::QuotedString s);
    void push(boxscript::ast::Date date);
    void push(boxscript::ast::Time time);
    void push(boxscript::ast::DateTime dt);
    void push(boxscript::ast::BareDate bd);
    void push(boxscript::ast::Reference ref);
    void push(boxscript::ast::GroupedExpression group);
    void push(boxscript::ast::FunctionCall func);
    void push(Value value);
    void push(Operator operatr);
    void push(Parenthesis parenthesis);
    void push(Path path);
    void push(FunctionCall func);
    void push(Comma comma);
    void close();
    void resolvePaths();
    void collectValues();
    Value evaluate();

    const Stack& original() const;
    const Stack& stack() const;
    const Element& at(int i) { return _stack.at(i); }
    static Type type(const Element& el) { return static_cast<Type>(el.index()); }

    QString originalAsString() const;
    QString stackAsString() const;

private:
    // Data
    QObject *_parent;
    Stack _stack, _original;
    bool _isClosed;
    // Methods
    void toPostfix();
    void checkNotClosed();
    void reduceByOperator(Stack &stack);
    void reduceByFunctionCall(Stack &stack);
    static QString toString(const Stack &stack);
    static QString toString(const Element &element);
};

}
#endif
