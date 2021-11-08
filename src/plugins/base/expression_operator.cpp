/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "exception.h"
#include "expression_operator.h"

namespace expression {

Precedence Precedence::_precedence;
QMap<Operator,int> Precedence::_precedences;

Arity Arity::_arity;
QMap<Operator,int> Arity::_arities;

QString toString(Operator op) {
    switch(op) {
    case Operator::Addition:        return "+";
    case Operator::Subtraction:     return "-";
    case Operator::Multiplication:  return "*";
    case Operator::Division:        return "/";
    case Operator::Exponentiation:  return "^";
    case Operator::And:             return "&&";
    case Operator::Or:              return "||";
    case Operator::Union:           return "|";
    case Operator::If:              return "?";
    case Operator::Else:            return ":";
    }
    ThrowException("Unexpected exception");
    return QString();
}


Precedence::Precedence() {
    _precedences[Operator::Addition] = 2;
    _precedences[Operator::Subtraction] = 2;
    _precedences[Operator::Multiplication] = 3;
    _precedences[Operator::Division] = 3;
    _precedences[Operator::Exponentiation] = 1;
    _precedences[Operator::And] = 11;
    _precedences[Operator::Or] = 12;
    _precedences[Operator::Union] = 10;
    _precedences[Operator::If] = 16;
    _precedences[Operator::Else] = 16;

}

int precedence(Operator op) {
    return Precedence::_precedences.value(op);
}

Arity::Arity() {
    _arities[Operator::Addition] = 2;
    _arities[Operator::Subtraction] = 2;
    _arities[Operator::Multiplication] = 2;
    _arities[Operator::Division] = 2;
    _arities[Operator::Exponentiation] = 1;
    _arities[Operator::And] = 2;
    _arities[Operator::Or] = 2;
    _arities[Operator::Union] = 2;
    _arities[Operator::If] = 1;
    _arities[Operator::Else] = 1;

}

int arity(Operator op) {
    return Arity::_arities.value(op);
}

}

