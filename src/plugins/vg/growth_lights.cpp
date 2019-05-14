/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "growth_lights.h"
#include "base/path.h"
#include <base/publish.h>
#include "growth_light.h"

using namespace base;

namespace vg {

PUBLISH(GrowthLights)

GrowthLights::GrowthLights(QString name, QObject *parent)
    : GrowthLightBase(name, parent)
{
    help("computes total emmision from all GrowthLight child boxes");

    port("shortWaveProp")->imports("./*<GrowthLight>[shortWaveProp]").transform(Sum);
    port("longWaveProp")->imports("./*<GrowthLight>[longWaveProp]").transform(Sum);
    port("heatProp")->imports("./*<GrowthLight>[heatProp]").transform(Sum);

    port("powerUsage")->imports("./*<GrowthLight>[powerUsage]").transform(Sum);
    port("totalIntensity")->imports("./*<GrowthLight>[totalIntensity]").transform(Sum);
    port("shortWaveIntensity")->imports("./*<GrowthLight>[shortWaveIntensity]").transform(Sum);
    port("longWaveIntensity")->imports("./*<GrowthLight>[longWaveIntensity]").transform(Sum);
    port("heatIntensity")->imports("./*<GrowthLight>[heatIntensity]").transform(Sum);

    port("parIntensity")->imports("./*<GrowthLight>[parIntensity]").transform(Sum);
    port("currentlyOn")->imports("./*<GrowthLight>[on]").transform(Any);
}

void GrowthLights::amend() {
    Box *light = findMaybeOne<Box>("./*<GrowthLight>");
    if (!light) {
        port("shortWaveProp")->equals(0);
        port("longWaveProp")->equals(0);
        port("heatProp")->equals(0);
        port("powerUsage")->equals(0);
        port("totalIntensity")->equals(0);
        port("shortWaveIntensity")->equals(0);
        port("longWaveIntensity")->equals(0);
        port("heatIntensity")->equals(0);
        port("parIntensity")->equals(0);
        port("currentlyOn")->equals(0);
    }
}

} //namespace

