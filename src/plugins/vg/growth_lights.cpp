/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "growth_lights.h"
#include "base/path.h"
#include <base/publish.h>
using namespace base;

namespace vg {

PUBLISH(GrowthLights)

GrowthLights::GrowthLights(QString name, QObject *parent)
    : GrowthLightBase(name, parent)
{
    help("computes total emmision from all GrowthLight child boxes");
    port("heatEmission")->imports("./*<GrowthLight>[heatEmission]").transform(Sum);
    port("longWaveEmission")->imports("./*<GrowthLight>[longWaveEmission]").transform(Sum);
    port("shortWaveEmission")->imports("./*<GrowthLight>[shortWaveEmission]").transform(Sum);
    port("totalEmission")->imports("./*<GrowthLight>[totalEmission]").transform(Sum);
    port("parEmission")->imports("./*<GrowthLight>[parEmission]").transform(Sum);
    port("photonIntensity")->imports("./*<GrowthLight>[photonIntensity]").transform(Sum);
    port("energyFlux")->imports("./*<GrowthLight>[energyFlux]").transform(Sum);
    port("currentlyOn")->imports("./*<GrowthLight>[on]").transform(Any);
    Output(currentPower).imports("./*<GrowthLight>[currentPower]").transform(Sum);
}

} //namespace

