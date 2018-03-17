/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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
    Input(length).imports("construction/geometry[length]").help("Length of one window (m)");
    Input(width).equals(0.5).help("Width of one window (m)");
    Input(number).equals(1).help("Number of windows");
    Input(state).help("Current relative opening [0;1]");
    Input(transmissivity).importsMaybe(pathTransmissivity);

    Output(area).help("Total area of windows (m2)");
    Output(effectiveArea).help("Total effective (corrected for screens) vent area (m2)");
    Output(maxEffectiveArea).help("Max. effective (corrected for screens, state==1) vent area (m2)");
}

void Vent::amend() {
    if (!findMaybeOne<Box>(pathTransmissivity))
        transmissivity = 1.;
}

void Vent::reset() {
    area = length*width*number;
}

void Vent::update() {
    maxEffectiveArea = transmissivity*area;
    effectiveArea = state*maxEffectiveArea;
}

} //namespace


