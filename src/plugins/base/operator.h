/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_OPERATOR_H
#define BASE_OPERATOR_H
#include <QMap>

namespace base {

enum class Operator {
    Add,
    Subtract,
    Multiply,
    Divide,
    Negate,
    Exponentiate,
    And,
    Or,
    Not
};

enum class Parenthesis {
    Left,
    Right
};

class Precedence {
    Precedence();
    static Precedence _precedence;
    static QMap<Operator,int> _precedences;
    friend int precedence(Operator op);
};
int precedence(Operator op);

class Arity {
    Arity();
    static Arity _arity;
    static QMap<Operator,int> _arities;
    friend int arity(Operator op);
};
int arity(Operator op);


}
#endif
