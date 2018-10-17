/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "growth_light_base.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(GrowthLightBase)

GrowthLightBase::GrowthLightBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(GrowthLightBase);
    help("base class for models of growth light");
    Output(shortWaveProp).help("Proportion of intensity emitted as short waves [0;1]");
    Output(longWaveProp).help("Proportion of intensity emitted as long waves [0;1]");
    Output(heatProp).help("Proportion of intensity emitted as convective heat [0;1]");

    Output(powerUsage).help("Current power usage [W/m2]");
    Output(totalIntensity).help("Intensity of power emitted by lamps [W/m2]");
    Output(shortWaveIntensity).help("Short wave intensity (visible light, Rn) [W/m2]");
    Output(longWaveIntensity).help("Long wave intensity (thermal radiation, Bn) [W/m2]");
    Output(heatIntensity).help("Convective heat emission [W/m2]");

    Output(parIntensity).help("Intensity of PAR [micromol/s/m2]");
    Output(currentlyOn).help("Tells whether light is on");
}

} //namespace

