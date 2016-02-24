/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "energy_flux_cooling_demand.h"
#include "general.h"
#include <base/publish.h>

using std::max;
using namespace base;

namespace vg {

PUBLISH(EnergyFluxCoolingDemand)

/*! \class EnergyFluxCoolingDemand
 * \brief Flux of energy demanded to cool the greenhouse down to the ventilation setpoint
 * Inputs
 * ------
 * - _heatedTemperature_ is the indoors temperature reached by passive fluxes + heating [<SUP>o</SUP>C]
 * - _setpointTemperature_ is the ventilation setpoint [<SUP>oC</SUP>C]
 * - _volume_ is the greenhouse total volume [m<SUP>3</SUP>]
 * - _area_ is the area covered by the greenhouse [m<SUP>2</SUP>]
 * - _timeStep_ is the integration time step [s]
 *
 * Output
 * ------
 * - _value_ is the flux of energy demanded to cool the greenhouse down to the ventilation setpoint [W/m<SUP>2</SUP>]
 */

EnergyFluxCoolingDemand::EnergyFluxCoolingDemand(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(heatedTemperature, "../../temperature[value]");
    Input(setpointTemperature, "setpoints/temperature/ventilation[value]");
    Input(volume, "geometry[indoorsVolume]");
    Input(area, "geometry[groundArea]");
    Input(timeStep, "calendar[timeStepSecs]");
    Output(value);
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

