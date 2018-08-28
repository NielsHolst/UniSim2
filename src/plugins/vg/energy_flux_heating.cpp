/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "energy_flux_heating.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(EnergyFluxHeating)

EnergyFluxHeating::EnergyFluxHeating(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    Input(density).imports("actuators/heating/pipes/*[density]");
    Input(diameter).imports("actuators/heating/pipes/*[diameter]");
    Input(flowRate).imports("actuators/heating/pipes/*[flowRate]");
    Input(a).imports("actuators/heating/pipes/*[a]");
    Input(b).imports("actuators/heating/pipes/*[b]");

    Input(inflowTemperature).imports("actuators/heating[value]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(groundArea).imports("geometry[groundArea]");

    Output(pipeLength).help("Total length of each pipe [m]");
    Output(pipeVolume).help("Total volume of each pipe [m3]");
    Output(transitTime).help("Average transit time of water in each pipe [s]");
    Output(outflowTemperature).help("Water temperature at exit from each pipe [oC]");
    Output(temperatureDrop).help("Drop in water temperature from entry to exit in each pipe [oC]");
    Output(energyFlux).help("Energy flux from each pipe [W/m2]");

}

void EnergyFluxHeating::reset() {
    n = density.size();

    pipeLength.clear();
    for (double d : density)
        pipeLength << d*groundArea;

    pipeVolume.clear();
    for (int i=0; i<n; ++i) {
        pipeVolume << PI/4.*sqr(diameter.at(i)/1000.)*pipeLength.at(i);
    }

    transitTime.clear();
    for (int i=0; i<n; ++i) {
        transitTime << pipeVolume.at(i)/flowRate.at(i)*1000.;
    }

    outflowTemperature.resize(n);
    temperatureDrop.resize(n);
    energyFlux.resize(n);
}

void EnergyFluxHeating::update() {
    for (int i=0; i<n; ++i)
        updatePipe(i);
    value = vector_op::sum(energyFlux);
}

void EnergyFluxHeating::updatePipe(int i) {
    double Vpipe = PI/4.*sqr(diameter.at(i)/10.)*100,  // cm3/m
           k = a.at(i)*diameter.at(i) / (CpWater/1000.) / Vpipe, // K/s
           x = k*(b.at(i)-1.)*transitTime.at(i) + pow(inflowTemperature-indoorsTemperature, 1-b.at(i));
    temperatureDrop[i] = inflowTemperature - indoorsTemperature - pow(x, 1./(1.-b.at(i)));
    outflowTemperature[i] = inflowTemperature - temperatureDrop.at(i);
    double E = CpWater*pipeVolume.at(i)*temperatureDrop.at(i)*1000.;
    energyFlux[i] = E/groundArea/transitTime.at(i);
}

} //namespace

