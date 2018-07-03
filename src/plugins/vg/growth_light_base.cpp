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
    Output(heatEmission).help("Convective heat emission [W/m2]");
    Output(longWaveEmission).help("Long wave emission (thermal radiation, Bn) [W/m2]");
    Output(shortWaveEmission).help("Short wave emission (visible light, Rn) [W/m2]");
    Output(totalEmission).help("Total = heatEmission + longWaveEmission + shortWaveEmission [W/m2]");
    Output(parEmission).help("PAR emission [W/m2]");
    Output(photonIntensity).help("PAR Photon intensity [micromole PAR/s/m2");
    Output(energyFlux).help("Intensity of power used by lamps [W/m2]");
    Output(currentlyOn).help("Tells whether light is on");
}

} //namespace

