/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PORT_MODE_H
#define BASE_PORT_MODE_H

#include "exception.h"

namespace base {

enum class PortMode{DefaultFixed, DefaultEvaluated, SetFixed, SetEvaluated};

template<class T> T convert(PortMode) {
    ThrowException("Cannot convert PortMode");
    return T();
}

template<> QString convert(PortMode mode);

}
#endif

