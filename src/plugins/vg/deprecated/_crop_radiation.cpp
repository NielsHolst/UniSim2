/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "crop_radiation.h"
#include "general.h"

using std::max;
using namespace base;

namespace vg {

PUBLISH(CropRadiation)

CropRadiation::CropRadiation(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes crop light budget");
    Input(kDiffuse).equals(0.8).help("Extinction coefficient for diffuse light").unit("[0;1]");
    Input(scattering).equals(0.2).help("Scattering coefficent for direct light").unit("[0;1]");
    Input(sinb).imports("calendar[sinb]").unit("[-1;1]");
    Input(lightDiffuse).imports("indoors/light[diffuse]").unit("W/m2");
    Input(lightDirect).imports("indoors/light[direct]").unit("W/m2");
    Input(absorptivityTop).imports("layers/top/photosynthesis[absorptivity]").unit("[0;1]");
    Input(absorptivityMiddle).imports("layers/middle/photosynthesis[absorptivity]").unit("[0;1]");
    Input(absorptivityBottom).imports("layers/bottom/photosynthesis[absorptivity]").unit("[0;1]");

    Output(kDirect).help("Extinction coefficient for direct light").unit("[0;1]");
    Output(kDirectDirect).help("Extinction coefficient for the direct component of direct light").unit("[0;1]");
    Output(diffuseReflectivity).help("Reflectivity of diffuse light").unit("[0;1]");
    Output(directReflectivity).help("Reflectivity of direct light").unit("[0;1]");
    Output(reflectivity).help("Net reflectivity").unit("[0;1]");
}

void CropRadiation::reset() {
    kDirect = kDirectDirect = kDiffuse;
    diffuseReflectivity = directReflectivity = reflectivity = 0;
}

void CropRadiation::update() {
    double sqv = sqrt(1-scattering);

    // Reflectivity of horizontal leaf angle distribution
    diffuseReflectivity = (1-sqv)/(1+sqv);

    // Reflectivity of spherical leaf angle distribution
    directReflectivity = 2*diffuseReflectivity/(1+1.6*sinb);

    // Extinction coefficient for direct component of direct light (KdirBL)
    kDirectDirect = (sinb==0.) ? 0. : 0.5/sinb*kDiffuse/(0.8*sqv);

    // Extinction coefficient for total direct light
    kDirect = kDirectDirect*sqv;

    // Total light reflectivity weighted by diffuse vs. direct light
    double diffuse = div0(lightDiffuse, lightDiffuse+lightDirect);
    reflectivity = diffuse*diffuseReflectivity + (1-diffuse)*directReflectivity;
}

} //namespace
