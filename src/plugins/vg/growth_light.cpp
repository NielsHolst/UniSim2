/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "growth_light.h"
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>

using namespace base;

namespace vg {

PUBLISH(GrowthLight)

GrowthLight::GrowthLight(QString name, QObject *parent)
    : GrowthLightBase(name, parent)
{
    Input(type).equals("HPS").help("Type of lamp, 'HPS' or 'LED'");
    Input(intensity).equals(40.).help("Power of installed lamps per greenhouse area (W/m2)");
    Input(parPhotonCoef).equals(1.6).help("Number of PAR photons per spent lamp energy (micromole/J)");
    Input(minPeriodOn).help("Minimum period that light stays on (m)");
    Input(age).help("Age of the lamps at simulation start (h)");
    Input(lifeTime).equals(12000.).help("Age at which light output is reduced to 50% (h)");
    Input(on).imports("controllers/growthLight[signal]");
    Input(timeStep).imports("calendar[timeStepSecs]");

    Output(currentPeriod).help("Time since last time light went on (m)");
    Output(totalPeriod).help("Total period when light has been on (h)");
}

void GrowthLight::reset() {
    // Set parameters according to type
    QString key = type.toLower();
    if (key == "hps" || key == "hpsl") {
        shortWaveProp = 0.42;
        longWaveProp = 0.50;
    }
    else if (key == "led") {
        shortWaveProp = 0.05;
        longWaveProp = 0.82;
    }
    else
        ThrowException("Unknown growth light type").value(type).context(this);

    if (TestNum::gt(shortWaveProp + longWaveProp, 1.))
        ThrowException("Sum of shortWaveProp and longWaveProp cannot exceed 1").
                value(shortWaveProp).value2(longWaveProp).context(this);

    heatProp = 1. - shortWaveProp - longWaveProp;

    _degradationRate = (lifeTime>0) ? log(0.5)/lifeTime : 0.;
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

        totalIntensity = intensity*exp(_degradationRate*(age+totalPeriod));
        shortWaveIntensity = totalIntensity*shortWaveProp;
        longWaveIntensity = totalIntensity*longWaveProp;
        heatIntensity = totalIntensity*heatProp;

        parIntensity = totalIntensity*parPhotonCoef;
        powerUsage = intensity;
    }
    else
        noLight();
}

void GrowthLight::noLight() {
    totalIntensity =
    shortWaveIntensity =
    longWaveIntensity =
    heatIntensity =

    parIntensity =
    powerUsage = 0.;
    currentlyOn = false;
}

} //namespace

