/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "energy_flux_cooling_supply.h"
#include "general.h"
#include <base/publish.h>

using std::min;
using std::max;
using namespace base;

namespace vg {

PUBLISH(EnergyFluxCoolingSupply)

EnergyFluxCoolingSupply::EnergyFluxCoolingSupply(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    help("computes the energy flux supplied to cool the greenhouse to the ventilation setpoint");
    Input(airSupplyMax).imports("cooling/airSupplyMax[value]");
    Input(energyDemand).imports("cooling/demand[value]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(height).imports("geometry[indoorsAverageHeight]");
    Input(airTransmissivity).imports("construction/shelters[airTransmissivity]");
}

void EnergyFluxCoolingSupply::update() {
    double dT = outdoorsTemperature - indoorsTemperature;
    if (dT < 0.) {
        // W/m2 = m * h-1 / (s/h) * K * J/kg/K * kg/m3
        double energySupplyMax = height*airSupplyMax*airTransmissivity/3600*dT*CpAir*RhoAir;
        value = max(energyDemand, energySupplyMax); // both are negative or zero
    }
    else {
        value = 0.;
    }
}

} //namespace

