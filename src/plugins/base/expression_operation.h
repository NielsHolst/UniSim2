/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef EXPRESSION_OPERATION_H
#define EXPRESSION_OPERATION_
#include "expression_operand.h"

namespace expression {
    Operand add(const Operand &a, const Operand &b);
    Operand subtract(const Operand &a, const Operand &b);
    Operand multiply(const Operand &a, const Operand &b);
    Operand divide(const Operand &a, const Operand &b);
}
#endif
