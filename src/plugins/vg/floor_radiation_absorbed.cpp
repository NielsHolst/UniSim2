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
    Input(lightAbsorbedByCrop).imports("crop/lightAbsorbed[value]");
    Input(growthLightLwAbsorbedByCrop).imports("crop/growthLightLwAbsorbed[value]");
    Output(value).help("Thermal radiation flux absorbed by the floor [W/m2]");
}

void FloorRadiationAbsorbed::update() {
    value = max(indoorsLight + growthLightLight - lightAbsorbedByCrop, 0.) +
            max(growthLightLw - growthLightLwAbsorbedByCrop, 0.);
}

} //namespace

