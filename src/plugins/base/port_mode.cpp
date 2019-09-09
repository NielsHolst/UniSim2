/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "port_mode.h"

namespace base {

#define CASE_PORT_MODE(x) case PortMode::x: return #x

template<> QString convert(base::PortMode mode) {
    switch (mode) {
        CASE_PORT_MODE(Default);
        CASE_PORT_MODE(Fixed);
        CASE_PORT_MODE(Referenced);
        CASE_PORT_MODE(MaybeReferenced);
    }
    return QString();
}

}

