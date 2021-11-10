/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef EXPRESSION_STACK_H
#define EXPRESSION_STACK_H
#include <stack>
#include <vector>
#include "expression_element.h"
#include "expression_operand.h"
#include "expression_operator.h"

namespace expression {

class Stack
{
public:
    Stack();
    void clear();
    void push(Element element);
    void push(Operator operator_);
    void push(Operand operand);
    void push(Parenthesis parenthesis);
    void evaluate();
    Operand value() { return _value; }
    const Element & at(int i) const;
    bool isEmpty() const;
    bool containsPortPaths() const;
    void toPostfix();
    QString toString() const;
private:
    // Data
    typedef std::vector<Element> Elements;
    typedef Elements::const_iterator ElementsIt;
    Elements _elements;
    std::vector<Operand> _stack;
    Operand _value;
    // Methods
    void reduce(Operator op);
};

}
#endif
