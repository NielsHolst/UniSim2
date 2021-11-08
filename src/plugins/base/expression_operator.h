/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef EXPRESSION_OPERATOR_H
#define EXPRESSION_OPERATOR_H
#include <QMap>

namespace expression {

enum class Operator {
    Addition,
    Subtraction,
    Multiplication,
    Division,
    Exponentiation,
    And,
    Or,
    Union,
    If,
    Else
};
QString toString(Operator op);

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



}
#endif
