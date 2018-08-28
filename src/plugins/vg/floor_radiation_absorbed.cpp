/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "floor_radiation_absorbed.h"
#include <base/publish.h>

using std::max;
using namespace base;

namespace vg {

PUBLISH(FloorRadiationAbsorbed)

FloorRadiationAbsorbed::FloorRadiationAbsorbed(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes thermal radiation absorbed by the floow");
    Input(indoorsLight).imports("indoors/light[total]");
    Input(growthLightLight).imports("actuators/growthLights[shortWaveEmission]");
    Input(reflectance).equals(0.5).help("Short-wave reflectance of floor[0;1]");
    Input(lightAbsorbedByCrop).imports("crop/lightAbsorbed[value]");
    Input(growthLightLwAbsorbedByCrop).imports("crop/growthLightLwAbsorbed[value]");
    Output(value).help("Radiation absorbed by the floor [W/m2]");
    Output(reflected).help("Radiation reflected by the floor [W/m2]");
}

void FloorRadiationAbsorbed::update() {
    double maxAbsorbed =
            max(indoorsLight + growthLightLight - lightAbsorbedByCrop, 0.) +
            max(growthLightLw - growthLightLwAbsorbedByCrop, 0.);
    reflected = reflectance*maxAbsorbed;
    value = maxAbsorbed - reflected;
}

} //namespace

