/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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
    Input(indoorsLight).imports("indoors/light[total]",CA).unit("W/m2");
    Input(growthLightLight).imports("actuators/growthLights[shortWaveIntensity]",CA).unit("W/m2");
    Input(reflectance).equals(0.5).help("Short-wave reflectance of floor[0;1]").unit("[0;1]");
    Input(lightAbsorbedByCrop).imports("crop/lightAbsorbed[value]",CA).unit("W/m2");
    Input(growthLightLwAbsorbedByCrop).imports("crop/growthLightLwAbsorbed[value]",CA).unit("W/m2");
    Output(value).help("Radiation absorbed by the floor").unit("W/m2");
    Output(reflected).help("Radiation reflected by the floor]").unit("W/m2");
}

void FloorRadiationAbsorbed::update() {
    double maxAbsorbed =
            max(indoorsLight + growthLightLight - lightAbsorbedByCrop, 0.) +
            max(growthLightLw - growthLightLwAbsorbedByCrop, 0.);
    reflected = reflectance*maxAbsorbed;
    value = maxAbsorbed - reflected;
}

} //namespace

