/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "growth_lights.h"
#include "base/path.h"
#include <base/publish.h>
using namespace base;

namespace vg {

PUBLISH(GrowthLights)

/*! \class GrowthLights
 * \brief The total output from all growth lights
 *
 * Sums the outputs from all GrowthLightBase'ed child models.
 *
 */

GrowthLights::GrowthLights(QString name, QObject *parent)
    : GrowthLightBase(name, parent)
{
    port("heatEmission")->imports("./*{GrowthLight}[heatEmission]").transform(Sum);
    port("longWaveEmission")->imports("./*{GrowthLight}[longWaveEmission]").transform(Sum);
    port("shortWaveEmission")->imports("./*{GrowthLight}[shortWaveEmission]").transform(Sum);
    port("totalEmission")->imports("./*{GrowthLight}[totalEmission]").transform(Sum);
    port("parEmission")->imports("./*{GrowthLight}[parEmission]").transform(Sum);
    port("energyFlux")->imports("./*{GrowthLight}[energyFlux]").transform(Sum);
    port("currentlyOn")->imports("./*{GrowthLight}[currentlyOn]").transform(Any);
}

} //namespace

