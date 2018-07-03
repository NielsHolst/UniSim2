/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "sky_temperature.h"

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(SkyTemperature)

SkyTemperature::SkyTemperature(QString name, QObject *parent)
	: Box(name, parent)
{
    help("computes sky temperature from temperature and RH");
    Input(intercept).equals(0.00635).help("Emissivity intercept on dew point temperature");
    Input(slope).equals(0.00635).help("Emissivity slope on dew point temperature");
    Input(airTemperature).imports("outdoors[temperature]");
    Input(rh).imports("outdoors[rh]");
    Output(temperature).help("Sky temperature (oC)");
    Output(emissivity).help("Sky emmisivity [0;1]");
}

void SkyTemperature::reset() {
    update();
}

void SkyTemperature::update() {
    double dewTemp = Tdew(airTemperature, rh);
    emissivity = intercept + slope*dewTemp;
    if (emissivity>1) emissivity = 1;
    temperature = pow(emissivity*p4(dewTemp+T0), 0.25) - T0;
}

} //namespace
