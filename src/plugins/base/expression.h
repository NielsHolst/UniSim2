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
        Parenthesis
    >;

    Expression();
    void clear();
    void push(Value value);
    void push(Operator operatr);
    void push(Parenthesis parenthesis);
    void toPostfix();
    Value evaluate();
    QString toString() const;
private:
    // Data
    using Stack = std::vector<Element>;
    Stack _stack;
    // Methods
    void reduce(Stack &stack);
};

}
#endif
