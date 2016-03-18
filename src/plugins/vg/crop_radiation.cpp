/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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


/*! \class CropRadiation
 * \brief Basic parameters for radiation in crop canopy
 *
 * Inputs
 * ------
 * - _kDiffuse_ is the extinction coefficient for diffuse light [0;1]
 * - _scattering_ is the scattering coefficent for direct light [0;1]
 * - _sinb_ is the sine of sun elevation [-1;1]
 * - _lightDiffuse_ is the intensity of diffuse light indoors [W/m<SUP>2</SUP>]
 * - _lightDirect_ is the intensity of direct light indoors [W/m<SUP>2</SUP>]
 * - _absorptivityTop_ is the proportion of indoors light captured by the top leaf layer [0;1]
 * - _absorptivityMiddel_ is the proportion of indoors light captured by the middle leaf layer [0;1]
 * - _absorptivityBottom_ is the proportion of indoors light captured by the bottom leaf layer [0;1]
 *
 * Output
 * ------
 * - _kDirect_ is the extinction coefficient for direct light [0;1]
 * - _kDirectDirect_ is the extinction coefficient for the direct component of direct light [0;1]
 * - _diffuseReflectivity_ is the reflectivity of diffuse light [0;1]
 * - _directReflectivity_ is the reflectivity of direct light [0;1]
 * - _reflectivity_ is the reflectivity of diffuse and direct light combined [0;1]
 */

CropRadiation::CropRadiation(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(kDiffuse).equals(0.8);
    Input(scattering).equals(0.2);
    Input(sinb).imports("calendar[sinb]");
    Input(lightDiffuse).imports("indoors/light[diffuse]");
    Input(lightDirect).imports("indoors/light[direct]");
    Input(absorptivityTop).imports("layers/top/photosynthesis[absorptivity]");
    Input(absorptivityMiddle).imports("layers/middle/photosynthesis[absorptivity]");
    Input(absorptivityBottom).imports("layers/bottom/photosynthesis[absorptivity]");

    Output(kDirect);
    Output(kDirectDirect);
    Output(diffuseReflectivity);
    Output(directReflectivity);
    Output(reflectivity);
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

