/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "output_ports.h"

using namespace base;

namespace boxes {

OutputPorts::OutputPorts(QString name, Box *parent)
    : Box(name, parent)
{
    Input(ports).help("Ports to include in output");
}


}
