/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <ctype.h>
#include <base/exception.h>
#include "list_options.h"

namespace command {

uint qHash(const ListOption &a) {
    return static_cast<int>(a);
}

}

using namespace command;

namespace base {

template<> ListOption convert(char c) {
    ListOption o;
    switch (c) {
    case 'p': o = ListOption::Ports; break;
    case 'i': o = ListOption::Inputs; break;
    case 'o': o = ListOption::Outputs; break;
    case 's': o = ListOption::Short; break;
    case 'm': o = ListOption::Imports; break;
    case 'x': o = ListOption::Exports; break;
    case 'b': o = ListOption::Boxes; break;
    case 'r': o = ListOption::Recurse; break;
    default: ThrowException("Unknown list option")
                .value(c).hint("Valid options: psiomxbr");
    }
    return o;
}

template<> ListOptionSet convert(QString s) {
    ListOptionSet ops;
    if (s.isEmpty())
        ops << ListOption::Boxes << ListOption::Recurse;
    else for (int i=0; i<s.size(); ++i)
        ops << convert<command::ListOption>(s.at(i).toLatin1());
    return ops;
}

}

