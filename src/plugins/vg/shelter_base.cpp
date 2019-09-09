/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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
    Output(diffuseLightTransmitted).help("Intensity of sunlight transmitted as diffuse light").unit("W/m2 ground");
    Output(directLightTransmitted).help("Intensity of sunlight transmitted as direct light").unit("W/m2 ground");
    Output(totalLightTransmitted).help("Intensity of sunlight transmitted in total (diffuse+direct)").unit("W/m2 ground");
    Output(lightAbsorbedCover).help("Intensity of sunlight absorbed by cover").unit("W/m2 ground");
    Output(lightAbsorbedScreens).help("Intensity of sunlight absorbed by screens").unit("W/m2 ground");
    Output(haze).help("Proportion of direct light becoming dispersed on passage through cover and screens").unit("[0;1]");
    Output(U).help("Combined U-value of cover and screens").unit("W/K/m2");
    Output(airTransmissivity).help("Proportion of air transmitted through cover and screens").unit("[0;1]");
}

void ShelterBase::reset() {
    resetRadiationOutputs();
    airTransmissivity = 1.;
}


} //namespace

