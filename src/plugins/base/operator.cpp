/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <map>
#include "operator.h"

using std::map;

namespace base {

static map<Operator,int> precedences =
{
   {Operator::Add          , 6},
   {Operator::Subtract     , 6},
   {Operator::Multiply     , 7},
   {Operator::Divide       , 7},
   {Operator::Negate       , 8},
   {Operator::Exponentiate , 9},
   {Operator::And          , 5},
   {Operator::Or           , 4},
   {Operator::Not          , 8}
};

static map<Operator,int> arities =
{
   {Operator::Add          , 2},
   {Operator::Subtract     , 2},
   {Operator::Multiply     , 2},
   {Operator::Divide       , 2},
   {Operator::Negate       , 1},
   {Operator::Exponentiate , 2},
   {Operator::And          , 2},
   {Operator::Or           , 2},
   {Operator::Not          , 1}
};

int precedence(Operator op) {
    return precedences.at(op);
}

int arity(Operator op) {
    return arities.at(op);
}

}

