/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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
    Input(heatingPowerUsage).imports("controlled/heating/energyFlux[value]",CA).unit("W/m2");
    Input(growthLightsPowerUsage).imports("actuators/growthLights[powerUsage]",CA).unit("W/m2");
    Input(co2Flux).imports("controllers/co2[signal]",CA).unit("g/m2/h");
    Input(dt).imports("calendar[timeStepSecs]").unit("s");
    Input(energyUnit).equals("GJ").help("Energy units for outputs").unit("GJ|kWh");
    Output(heatingEnergyTotal).help("Accumulated energy spent on heating");
    Output(growthLightsEnergyTotal).help("Accumulated energy spent on growth lights");
    Output(co2Total).help("Accumulated CO2 spent").unit("kg/m2");
}

void Budget::initialize() {
    QString u = energyUnit.toLower();
    if (u=="gj") {
        port("heatingEnergyTotal")->unit("GJ");
        port("growthLightsEnergyTotal")->unit("GJ");
    }
    else if (u=="kwh") {
        port("heatingEnergyTotal")->unit("kWh");
        port("growthLightsEnergyTotal")->unit("kWh");
    }
}

void Budget::reset() {
    heatingPowerUsage = heatingEnergyTotal =
    growthLightsPowerUsage = growthLightsEnergyTotal =
    co2Flux = co2Total = 0.;
    QString u = energyUnit.toLower();
    if (u=="gj") {
        eUnit = dt*1e-9;          // Gs = s * G
    }
    else if (u=="kwh") {
        eUnit = dt/3600.*1e-3;    // kh = s / (s/h) * k
    }
    else {
        QString msg {"Unknown energy unit '%1', expected 'kWh' or 'GJ'"};
        ThrowException(msg.arg(energyUnit)).value(energyUnit).context(this);
    }
}

void Budget::update() {
    heatingEnergyTotal += heatingPowerUsage*eUnit;
    growthLightsEnergyTotal += growthLightsPowerUsage*eUnit;
    co2Total += co2Flux*dt/3600.*1e-3;   // g/m2/h * s / (s/h) * k
}


} //namespace

