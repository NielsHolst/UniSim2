/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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
    Input(type).equals("HPS").help("Type of lamp").unit("HPS|LED");
    Input(intensity).help("Power of installed lamps per greenhouse area").unit("W/m2");
    Input(parPhotonCoef).equals(1.6).help("Number of PAR photons per spent lamp energy").unit("micromole/J");
    Input(minPeriodOn).help("Minimum period that light stays on").unit("m");
    Input(ageCorrectedEfficiency).equals(1.).help("Proportion of intensity actually emitted").unit("[0;1]");
    Input(on).imports("controllers/growthLight[signal]").unit("y|n");
    Input(timeStep).imports("calendar[timeStepSecs]").unit("s");

    Output(currentPeriod).help("Time since last time light went on").unit("m");
    Output(totalPeriod).help("Total period when light has been on").unit("h");
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

        totalIntensity = intensity*ageCorrectedEfficiency;
        shortWaveIntensity = totalIntensity*shortWaveProp;
        longWaveIntensity = totalIntensity*longWaveProp;
        heatIntensity = totalIntensity*heatProp;

        parIntensity = totalIntensity*parPhotonCoef;
        if (powerUsage==0.)
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

