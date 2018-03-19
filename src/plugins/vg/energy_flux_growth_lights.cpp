/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "energy_flux_growth_lights.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(EnergyFluxGrowthLights)

EnergyFluxGrowthLights::EnergyFluxGrowthLights(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    help("models energy flux caused by growth lights");
    Input(energyFlux).imports("actuators/growthLights[energyFlux]");
}

void EnergyFluxGrowthLights::update() {
    value = energyFlux;
}

} //namespace

