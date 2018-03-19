/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "cover.h"
#include "general.h"
#include <base/publish.h>
#include "screens.h"
#include "shelter_base.h"

using namespace base;

namespace vg {

ShelterBase::ShelterBase(QString name, QObject *parent)
    : SurfaceRadiationOutputs(name, parent)
{
    Class(ShelterBase);
    help("is a base class for the ShelterFace's and the whole Shelter");
    Output(diffuseLightTransmitted).help("Intensity of sunlight transmitted as diffuse light [W/ground m2]");
    Output(directLightTransmitted).help("Intensity of sunlight transmitted as direct light [W/ground m2]");
    Output(totalLightTransmitted).help("Intensity of sunlight transmitted in total (diffuse+direct) [W/ground m2]");
    Output(lightAbsorbedCover).help("Intensity of sunlight absorbed by cover [W/ground m2]");
    Output(lightAbsorbedScreens).help("Intensity of sunlight absorbed by screens [W/ground m2]");
    Output(haze).help("Proportion of direct light becoming dispersed on passage through cover and screens [0;1]");
    Output(U).help("Combined U-value of cover and screens [W/m2/K]");
    Output(airTransmissivity).help("Proportion of air transmitted through cover and screens [0;1]");
}

void ShelterBase::reset() {
    resetRadiationOutputs();
    airTransmissivity = 1.;
}


} //namespace

