/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "energy_flux_heating_demand.h"
#include "general.h"
#include <base/publish.h>

using namespace base;
using std::max;

namespace vg {

PUBLISH(EnergyFluxHeatingDemand)

EnergyFluxHeatingDemand::EnergyFluxHeatingDemand(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    help("computes the energy flux demanded to heat the greenhouse to the heating setpoint");
    Input(givenEnergyFlux).imports("given/energyFlux[value]");
    Input(heatingSetpoint).imports("setpoints/temperature/heating[value]");
    Input(indoorsTemperature).imports("indoors[temperature]");
    Input(height).imports("geometry[indoorsAverageHeight]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(noHeating).imports("indoors[noHeating]");
}

void EnergyFluxHeatingDemand::update() {
    double dT = max(heatingSetpoint - indoorsTemperature, 0.);
    value = noHeating ? 0. : max(dT*CpAir*RhoAir*height/timeStep - givenEnergyFlux, 0.);
}

} //namespace
