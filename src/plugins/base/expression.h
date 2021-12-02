/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_EXPRESSION_H
#define BASE_EXPRESSION_H
#include <stack>
#include <variant>
#include <vector>
#include "operator.h"
#include "value.h"

namespace base {

class Expression
{
public:
    using Element = std::variant<
        Value,
        Operator,
        Parenthesis,
        QString         // Unresolved path; must be resolved to Value before evaluation
    >;
    enum class Type {
        Value,
        Operator,
        Parenthesis,
        Path
    };
    using Stack = std::vector<Element>;

    Expression();
    void clear();
    int size() const     { return _stack.size();}
    bool isEmpty() const { return _stack.empty();}
    void push(Value value);
    void push(Operator operatr);
    void push(Parenthesis parenthesis);
    void push(QString path);
    void close();
    void resolvePaths();
    void toPostfix();
    Value evaluate();

    const Stack& original() const;
    const Stack& stack() const;
    const Element& at(int i) { return _stack.at(i); }
    static Type type(const Element& el) { return static_cast<Type>(el.index()); }

    QString originalAsString() const;
    QString stackAsString() const;

private:
    // Data
    Stack _stack, _original;
    bool _isClosed;
    // Methods
    void checkNotClosed();
    void reduce(Stack &stack);
    static QString toString(const Stack &stack);
};

}
#endif
