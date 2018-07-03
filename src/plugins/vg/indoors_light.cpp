/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "indoors_light.h"
#include <base/publish.h>
#include "shelter.h"

using namespace base;

namespace vg {
	
PUBLISH(IndoorsLight)

IndoorsLight::IndoorsLight(QString name, QObject *parent)
	: Box(name, parent)
{
    help("computes indoors diffuse and direct light and PAR");
    Input(sunlightDiffuse).imports("construction/shelter[diffuseLightTransmitted]");
    Input(sunlightDirect).imports("construction/shelter[directLightTransmitted]");
    Input(sunlightPropPar).imports("outdoors[propParRadiation]");
    Input(growthLigthtsDirect).imports("growthLights[shortWaveEmission]");
    Input(growthLigthtsPar).imports("growthLights[parEmission]");
    Input(growthLigthtsPhotonIntensity).imports("growthLights[photonIntensity]");
    Input(sunlightPhotonCoef).equals(4.6).help("Number of PAR photons in sunlight energy (micromole/J)");
    Output(direct).help("Intensity of indoors direct light [W/m2]");
    Output(diffuse).help("Intensity of indoors diffuse light [W/m2]");
    Output(total).help("Intensity of indoors direct+diffuse light [W/m2]");
    Output(parDirect).help("Intensity of indoors direct PAR [W/m2]");
    Output(parDiffuse).help("Intensity of indoors diffuse PAR [W/m2]");
    Output(parTotal).help("Intensity of indoors direct+diffuse PAR [W/m2]");
    Output(photonIntensity).help("PAR Photon intensity [micromole PAR/s/m2]");
}

void IndoorsLight::reset() {
    diffuse = direct = total =
    parDiffuse = parDirect = parTotal = 0.;
}

void IndoorsLight::update() {
    diffuse = sunlightDiffuse;
    direct = sunlightDirect + growthLigthtsDirect;
    total = direct + diffuse;

    parDiffuse = sunlightPropPar*diffuse;
    parDirect = sunlightPropPar*direct + growthLigthtsPar;
    parTotal = parDiffuse + parDirect;

    photonIntensity = (sunlightDiffuse + sunlightDirect)*sunlightPhotonCoef + growthLigthtsPhotonIntensity;
}

} //namespace
