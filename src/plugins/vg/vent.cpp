/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <math.h>
#include <base/exception.h>
#include <base/publish.h>
#include "general.h"
#include "vent.h"

using namespace base;

namespace vg {
	
PUBLISH(Vent)

Vent::Vent(QString name, QObject *parent)
    : Box(name, parent),
      pathTransmissivity("../screens[airTransmissivity]")
{
    help("computes vent effective area");
    Input(length).imports("construction/geometry[length]").help("Length of one window (m)");
    Input(width).equals(1).help("Width of one window (m)");
    Input(number).equals(1).help("Number of windows");
    Input(state).imports("actuators/vents[value]").help("Relative vent opening [0;1]");
    Input(transmissivity).importsMaybe(pathTransmissivity);
}

void Vent::amend() {
    if (!findMaybeOne<Box>(pathTransmissivity))
        transmissivity = 1.;
}

} //namespace


