/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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
    Input(sunlightPhotonCoef).imports("outdoors[sunlightPhotonCoef]").unit("micromole/J");
    Input(sunlightDiffuse).imports("construction/shelter[diffuseLightTransmitted]").unit("W/m2");
    Input(sunlightDirect).imports("construction/shelter[directLightTransmitted]").unit("W/m2");
    Input(growthLigthtsDirect).imports("growthLights[shortWaveIntensity]").unit("W/m2");
    Input(growthLigthtsParIntensity).imports("growthLights[parIntensity]").unit("micromole/m2/s");
    Output(direct).help("Intensity of indoors direct light").unit("W/m2");
    Output(diffuse).help("Intensity of indoors diffuse light").unit("W/m2");
    Output(total).help("Intensity of indoors direct+diffuse light").unit("W/m2");
    Output(parDirect).help("Intensity of indoors direct PAR").unit("micromole/m2/s");
    Output(parDiffuse).help("Intensity of indoors diffuse PAR").unit("micromole/m2/s");
    Output(parTotal).help("Intensity of indoors direct+diffuse PAR").unit("micromole/m2/s");
}

void IndoorsLight::reset() {
    diffuse = direct = total =
    parDiffuse = parDirect = parTotal = 0.;
}

void IndoorsLight::update() {
    diffuse = sunlightDiffuse;
    direct = sunlightDirect + growthLigthtsDirect;
    total = direct + diffuse;

    parDiffuse = sunlightPhotonCoef*diffuse;
    parDirect = sunlightPhotonCoef*direct + growthLigthtsParIntensity;
    parTotal = parDiffuse + parDirect;
}

} //namespace
