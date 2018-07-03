/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "growth_light.h"
#include <base/exception.h>
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(GrowthLight)

GrowthLight::GrowthLight(QString name, QObject *parent)
    : GrowthLightBase(name, parent)
{
    Input(intensity).equals(40.).help("Power of installed lamps per greenhouse area (W/m2)");
    Input(ballastCorrection).equals(1.).help("Set to >1 if intensity includes ballast");
    Input(heatCoef).help("Proportion of net intensity lost by convection [0;1]");
    Input(longWaveCoef).help("Proportion of net intensity emitted as long-wave radiation [0;1]");
    Input(shortWaveCoef).help("Proportion of net intensity emitted as short-wave radiation [0;1]");
    Input(parCoef).help("Proportion of net intensity emitted as long-wave radiation [0;1]");
    Input(parPhotonCoef).equals(4.6).help("Number of photons in PAR energy (micromole/J)");
    Input(minPeriodOn).help("Minimum period that light stays on (m)");
    Input(age).help("Age of the lamps at simulation start (h)");
    Input(lifeTime).equals(12000.).help("Age at which light output is reduced to 50% (h)");
    Input(on).imports("controllers/growthLight[signal]");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(netIntensity).help("Intensity corrected for ballast (W/m2)");
    Output(currentPower).help("Current power usage (W/m2)");
    Output(currentPeriod).help("Time since last time light went on (m)");
    Output(totalPeriod).help("Total period when light has been on (h)");
}

void GrowthLight::reset() {
    // Set parameters according to type
//    QString key = type.toLower();
//    if (key == "hpsl") {
//        attributes.heatCoef = 0.23;
//        attributes.longWaveCoef = 0.42;
//        attributes.shortWaveCoef = 0.50;
//        attributes.parCoef = 0.31;
//        attributes.minPeriodOn = 30.;
//    }
//    else if (key == "led") {
//        attributes.heatCoef = 0.02;
//        attributes.longWaveCoef = 0.05;
//        attributes.shortWaveCoef = 0.82;
//        attributes.parCoef = 0.82;
//        attributes.minPeriodOn = 0.;
//    }
//    else
//        ThrowException("Unknown growth light type").value(type).context(this);
    // Compute derived parameters
    double netCapacity = intensity/ballastCorrection;
    netAttributes.heatEmission = heatCoef*netCapacity;
    netAttributes.longWaveEmission = longWaveCoef*netCapacity;
    netAttributes.shortWaveEmission = shortWaveCoef*netCapacity;
    netAttributes.parEmission = parCoef*netCapacity;
    netAttributes.energyFlux = ballastCorrection*intensity;
    degradationRate = (lifeTime>0) ? log(0.5)/lifeTime : 0.;
    currentPeriod = 0.;
    totalPeriod = age;
    noLight();
}

void GrowthLight::update() {
    currentlyOn = on ||
                  ( currentlyOn && (currentPeriod < minPeriodOn) );
    if (currentlyOn) {
        currentPeriod += timeStep/60.;
        totalPeriod += timeStep/3600.;
        double f = exp(degradationRate*(age+totalPeriod));
        heatEmission = netAttributes.heatEmission + (1-f)*(netAttributes.longWaveEmission+netAttributes.shortWaveEmission);
        longWaveEmission = f*netAttributes.longWaveEmission;
        shortWaveEmission = f*netAttributes.shortWaveEmission;
        totalEmission = heatEmission + longWaveEmission + shortWaveEmission;
        parEmission = f*netAttributes.parEmission;
        photonIntensity = parEmission*parPhotonCoef;
        energyFlux = netAttributes.energyFlux;
        currentPower = currentlyOn ? intensity : 0.;
    }
    else
        noLight();
}

void GrowthLight::noLight() {
    heatEmission =
    longWaveEmission =
    shortWaveEmission =
    totalEmission =
    parEmission =
    photonIntensity =
    energyFlux =
    currentPower = 0.;
    currentlyOn = false;
}

} //namespace

