/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "port_access.h"

namespace base {

#define CASE_PORT_ACCESS(x) case PortAccess::x: return #x

template<> QString convert(base::PortAccess access) {
    switch (access) {
        CASE_PORT_ACCESS(Input);
        CASE_PORT_ACCESS(Output);
    }
    return QString();
}

}

