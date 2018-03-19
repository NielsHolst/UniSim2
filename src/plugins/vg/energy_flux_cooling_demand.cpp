/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "energy_flux_cooling_demand.h"
#include "general.h"
#include <base/publish.h>

using std::max;
using namespace base;

namespace vg {

PUBLISH(EnergyFluxCoolingDemand)

EnergyFluxCoolingDemand::EnergyFluxCoolingDemand(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    help("computes the energy flux demanded to cool the greenhouse to the ventilation setpoint");
    Input(heatedTemperature).imports("../../temperature[value]");
    Input(setpointTemperature).imports("setpoints/temperature/ventilation[value]");
    Input(volume).imports("geometry[indoorsVolume]");
    Input(area).imports("geometry[groundArea]");
    Input(timeStep).imports("calendar[timeStepSecs]");
}

void EnergyFluxCoolingDemand::update() {
    double dT = setpointTemperature - heatedTemperature;
    if (dT < 0.) {
        double dE = volume*RhoAir*CpAir*dT;     // J = m3 * kg/m3 * J/kg/K * K
        value = dE/area/timeStep;               // W/m2 = J / m2 / s
    }
    else {
        value = 0;
    }
}

} //namespace

