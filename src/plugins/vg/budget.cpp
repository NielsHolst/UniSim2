/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "budget.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(Budget)

Budget::Budget(QString name, QObject *parent)
	: Box(name, parent)
{
    help("computes greenhouse energy and CO2 budgets");
    Input(heatingEnergyFlux).imports("controlled/heating/energyFlux[value]");
    Input(growthLightsEnergyFlux).imports("actuators/growthLights[energyFlux]");
    Input(co2Flux).imports("controllers/co2[signal]");
    Input(dt).imports("calendar[timeStepSecs]");
    Input(energyUnit).equals("GJ").help("Energy units for outpurs, 'GJ' or 'kWh'");
    Output(heatingEnergyTotal).help("Accumulated energy spent on heating [GJ/m2 or kWh/m2]");
    Output(growthLightsEnergyTotal).help("Accumulated energy spent on growth lights [GJ/m2 or kWh/m2]");
    Output(co2Total).help("Accumulated CO2 spent [kg/m2]");
}

void Budget::reset() {
    heatingEnergyFlux = heatingEnergyTotal =
    growthLightsEnergyFlux = growthLightsEnergyTotal =
    co2Flux = co2Total = 0.;
    QString u = energyUnit.toLower();
    if (u=="gj")
        eUnit = dt*1e-9;          // Gs = s * G
    else if (u=="kwh")
        eUnit = dt/3600.*1e-3;    // kh = s / (s/h) * k
    else {
        QString msg {"Unknown energy unit '%1', expected 'kWh' or 'GJ'"};
        ThrowException(msg.arg(energyUnit)).value(energyUnit).context(this);
    }
}

void Budget::update() {
    heatingEnergyTotal += heatingEnergyFlux*eUnit;
    growthLightsEnergyTotal += growthLightsEnergyFlux*eUnit;
    co2Total += co2Flux*dt/3600.*1e-3;   // g/m2/h * s / (s/h) * k
}


} //namespace

