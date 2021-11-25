/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_OPERATE_H
#define BASE_OPERATE_H
#include "value.h"

namespace operate {
base::Value add         (const base::Value &a, const base::Value &b);
base::Value subtract    (const base::Value &a, const base::Value &b);
base::Value multiply    (const base::Value &a, const base::Value &b);
base::Value divide      (const base::Value &a, const base::Value &b);
base::Value exponentiate(const base::Value &a, const base::Value &b);
base::Value and_        (const base::Value &a, const base::Value &b);
base::Value or_         (const base::Value &a, const base::Value &b);
base::Value not_        (const base::Value &a);
base::Value negate      (const base::Value &a);
}

inline base::Value operator+(const base::Value &a, const base::Value &b) {
    return operate::add(a, b);
}

inline base::Value operator-(const base::Value &a, const base::Value &b) {
    return operate::add(a, b);
}

inline base::Value operator*(const base::Value &a, const base::Value &b) {
    return operate::multiply(a, b);
}

inline base::Value operator/(const base::Value &a, const base::Value &b) {
    return operate::divide(a, b);
}

inline base::Value operator&&(const base::Value &a, const base::Value &b) {
    return operate::and_(a, b);
}

inline base::Value operator||(const base::Value &a, const base::Value &b) {
    return operate::or_(a, b);
}

inline base::Value operator!(const base::Value &a) {
    return operate::not_(a);
}

#endif

