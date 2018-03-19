/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/path.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "general.h"
#include "pipe.h"

using std::min;
using std::max;
using base::bounded;

using namespace base;

namespace vg {
	
PUBLISH(Pipe)

Pipe::Pipe(QString name, QObject *parent)
	: Box(name, parent)
{
    help("models heat pipe");
    Input(length).equals(1.).help("Pipe length per greenhouse area [m/m2]");
    Input(diameter).equals(51.).help("Pipe inner diameter [mm]");
    Input(minTemperature).equals(20.).help("Minimum pipe temperature [oC]");
    Input(maxTemperature).equals(80.).help("Maximum pipe temperature [oC]");
    Input(maxTemperatureIncreaseRate).equals(2.).help("Maximum rate of increase in pipe temperature [oC/min]");
    Input(emissivity).equals(0.9).help("Pipe emissivity [0;1]");
    Input(energyFluxTotal).imports("heating/supply[value]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(timeStep).imports("calendar[timeStepSecs]");

    Output(temperature).help("Pipe temperature [oC]");
    Output(energyFlux).help("Energy flux from pipe [W/m2]");
    Output(nextTemperatureMin).help("Minimum pipe temperature possible in the next time step [oC]");
    Output(nextTemperatureMax).help("Maximum pipe temperature possible in the next time step [oC]");
    Output(nextEnergyFluxMin).help("Minimum pipe energy flux possible in the next time step [W/m2]");
    Output(nextEnergyFluxMax).help("Maximum pipe energy flux possible in the next time step [W/m2]");
}

void Pipe::initialize() {
    QVector<Box*> preceedingPipes = findMany<Box>("preceedingsibling::*");
    for (Box *pipe : preceedingPipes) {
        energyFluxFromPreceedingPipes << pipe->port("energyFlux")->valuePtr<double>();
    }
}

void Pipe::reset() {
    double d = bounded(26., diameter, 51.);
    slope = length*(0.0131*d + 0.105);
//    temperature = minTemperature;
    update();
}

void Pipe::update() {
    // Set temperature
    temperature = bounded(minTemperature,
                          indoorsTemperature +
                          calcTemperatureDifference(energyFluxTotal - energyFluxFromPreceedingPipesSum()),
                          maxTemperature);
    nextTemperatureMax = bounded(minTemperature,
                                 temperature + maxTemperatureIncreaseRate*timeStep/60.,
                                 maxTemperature);
    setNextTemperatureMin();
    // Set energy flux
    energyFlux = calcEnergyFlux(temperature - indoorsTemperature);
    nextEnergyFluxMax = calcEnergyFlux(nextTemperatureMax - indoorsTemperature);
    nextEnergyFluxMin = calcEnergyFlux(nextTemperatureMin - indoorsTemperature);
}

//! Heat flux from heat pipe [W/m<SUP>2</SUP>]
double Pipe::calcEnergyFlux(double temperatureDifference) const {
    // flux = slope*Tdiff^1.25
    return TestNum::leZero(temperatureDifference) ? 0. : slope*pow(temperatureDifference,exponent);
}

//! Temperature difference to obtain a certain heat flux [K]
double Pipe::calcTemperatureDifference(double energyFlux) const {
    // Tdiff = (flux/slope)^(1/1.25)
    return TestNum::leZero(energyFlux) ? 0. :pow(energyFlux/slope, 1./exponent);
}

void Pipe::setNextTemperatureMin() {
    double energyFlux = calcEnergyFlux(temperature - indoorsTemperature),
           waterVolume = length*PI*sqr(diameter/1000./2.);
    nextTemperatureMin = bounded(minTemperature,
                                 temperature - energyFlux*timeStep/CpWaterVol/waterVolume,    // K = W/m2 * s * K*m3/J * m2/m3
                                 maxTemperature);
}

double Pipe::energyFluxFromPreceedingPipesSum() {
    double sum{0};
    for (const double *energyFlux : energyFluxFromPreceedingPipes) {
        sum += (*energyFlux);
    }
    return sum;
}


} //namespace


