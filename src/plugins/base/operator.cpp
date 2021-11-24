/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "exception.h"
#include "operator.h"

namespace base {

Precedence Precedence::_precedence;
QMap<Operator,int> Precedence::_precedences;

Arity Arity::_arity;
QMap<Operator,int> Arity::_arities;

Precedence::Precedence() {
    _precedences[Operator::Add] = 2;
    _precedences[Operator::Subtract] = 2;
    _precedences[Operator::Multiply] = 3;
    _precedences[Operator::Divide] = 3;
    _precedences[Operator::Negate] = 2;
    _precedences[Operator::Exponentiate] = 1;
    _precedences[Operator::And] = 11;
    _precedences[Operator::Or] = 12;
    _precedences[Operator::Not] = 2;
}

int precedence(Operator op) {
    return Precedence::_precedences.value(op);
}

Arity::Arity() {
    _arities[Operator::Add] = 2;
    _arities[Operator::Subtract] = 2;
    _arities[Operator::Multiply] = 2;
    _arities[Operator::Divide] = 2;
    _arities[Operator::Negate] = 1;
    _arities[Operator::Exponentiate] = 1;
    _arities[Operator::And] = 2;
    _arities[Operator::Or] = 2;
    _arities[Operator::Not] = 1;
}

int arity(Operator op) {
    return Arity::_arities.value(op);
}

}

