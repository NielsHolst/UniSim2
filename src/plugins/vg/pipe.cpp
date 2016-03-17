/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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

/*! \class Pipe
 * \brief Heat pipe temperature and energy flux
 *
 * The model automatically looks up other Pipe models among its sibling models. This model aims to supply the energy flux,
 * not provided by preceeding Pipe models, to meet the over all goal of _energyFluxTotal_.
 *
 * Inputs
 * ------
 * - _length_ is pipe length per greenhouse area [m/m<SUP>2</SUP>]
 * - _diameter_ is the pipe inner diameter [mm]
 * - _minTemperature_ is the minimum pipe temperature [<SUP>o</SUP>C]
 * - _maxTemperature_ is the maximum pipe temperature [<SUP>o</SUP>C]
 * - _maxTemperatureIncreaseRate_ is the maximum rate at which pipe temperature can increase[<SUP>o</SUP>C/min]
 * - _emissivity_ is the emissivity of the heat pipe [0;1]
 * - _energyFluxTotal_ is the total energy flux to be produced by all pipes [W/m<SUP>2</SUP>]
 * - _indoorsTemperature_ is the indoors temperature [<SUP>o</SUP>C]
 * - _timeStep_ is the integration time step [s]
 *
 * Outputs
 * ------
 * - _temperature_ is the pipe temperature [<SUP>o</SUP>C]
 * - _energyFlux_ is the energy flux coming from this pipe [W/m<SUP>2</SUP>]
 * - _nextTemperatureMin_ is the minimum pipe temperature possible in the next time step [<SUP>o</SUP>C]
 * - _nextTemperatureMax_ is the maximum pipe temperature possible in the next time step [<SUP>o</SUP>C]
 * - _nextEnergyFluxMin_ is the minimum energyFLux possible in the next time step [W/m<SUP>2</SUP>]
 * - _nextEnergyFluxMax_ is the maximum energyFLux possible in the next time step [W/m<SUP>2</SUP>]
 */

Pipe::Pipe(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(length).equals(1.);
    Input(diameter).equals(51.);
    Input(minTemperature).equals(20.);
    Input(maxTemperature).equals(80.);
    Input(maxTemperatureIncreaseRate).equals(2.);
    Input(emissivity).equals(0.9);
    Input(energyFluxTotal).imports("heating/supply[value]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(timeStep).imports("calendar[timeStepSecs]");

    Output(temperature);
    Output(energyFlux);
    Output(nextTemperatureMin);
    Output(nextTemperatureMax);
    Output(nextEnergyFluxMin);
    Output(nextEnergyFluxMax);
}

void Pipe::initialize() {
    QVector<Box*> preceedingPipes = resolveMany<Box>("preceedingsibling::*");
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


