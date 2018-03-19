/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/exception.h>
#include <base/publish.h>
#include "general.h"
#include "air_flux_cooling_supply.h"

using namespace base;
using std::min;

namespace vg {

PUBLISH(AirFluxCoolingSupply)

AirFluxCoolingSupply::AirFluxCoolingSupply(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes supply of air flux to cool greenhouse");
    Input(energyFlux).imports("energyFlux/cooling/supply[value]");
    Input(airSupplyMax).imports("cooling/airSupplyMax[value]");
    Input(indoorsHeight).imports("geometry[indoorsAverageHeight]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Output(value).help("Relative rate of air exchange [m3/m3/h]");
}

void AirFluxCoolingSupply::reset() {
    value =  0;
}

void AirFluxCoolingSupply::update() {
    double dT = outdoorsTemperature - indoorsTemperature;
    if (dT < 0.) {
        // W*h/m2/K = m * J/kg/K * kg/m3 * h/s
        double C = indoorsHeight*CpAir*RhoAir/3600;
        // h-1 = W/m2 / (W*h/m2/K) / K = W/m2 * K*m2/(W*h) / K
        value = min(energyFlux/C/dT, airSupplyMax);
        Q_ASSERT(value >= 0.);
    }
    else {
        value = 0;
    }
}

//void VentilationForCoolingBottom::update() {
//    double dToutdoors = gap*(outdoorsTemperature - indoorsTemperature),
//           dTtop = (1-gap)*(topTemperature - indoorsTemperature);
//    // W*h/m2/K = m * J/kg/K * kg/m3 * h/s
//    double C = averageHeight*CpAir*RhoAir/3600;
//    // h-1 = W/m2 / (W*h/m2/K) / K = W/m2 * K*m2/(W*h) / K
//    fromOutdoors = energyFlux/C/dToutdoors;
//    fromTop = energyFlux/C/dTtop;
//}

} //namespace

