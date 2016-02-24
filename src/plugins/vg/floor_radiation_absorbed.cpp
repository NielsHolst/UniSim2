/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "floor_radiation_absorbed.h"
#include <base/publish.h>

using std::max;
using namespace base;

namespace vg {

PUBLISH(FloorRadiationAbsorbed)


/*! \class FloorRadiationAbsorbed
 * \brief
 *
 * Inputs
 * ------

 * Output
 * ------
 *
 */

FloorRadiationAbsorbed::FloorRadiationAbsorbed(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(indoorsLight, "indoors/light[total]");
    Input(growthLightLight, "actuators/growthlights[shortWaveEmission]");
    Input(lightAbsorbedByCrop, "crop/lightAbsorbed[value]");
    Input(growthLightLwAbsorbedByCrop, "crop/growthLightLwAbsorbed[value]");
    Output(value);
}

void FloorRadiationAbsorbed::reset() {
    value = 0.;
}

void FloorRadiationAbsorbed::update() {
    value = max(indoorsLight + growthLightLight - lightAbsorbedByCrop, 0.) +
            max(growthLightLw - growthLightLwAbsorbedByCrop, 0.);
}

} //namespace

