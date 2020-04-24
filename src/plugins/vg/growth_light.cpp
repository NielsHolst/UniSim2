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
    : RadiationLayer(name, parent)
{
    help("models the radiative emittance from growth lights");
    Input(on).help("Is light currently switched on?").unit("y|n");
    Input(intensity).help("Power of installed lamps per greenhouse area").unit("W/m2");
    Input(parPhotonCoef).equals(1.6).help("Number of PAR photons per spent lamp energy").unit("micromole/J");
    Input(propLw).equals(0.1).help("Proportion of power emitted as long-wave radiation").unit("[0;1]");
    Input(ageCorrectedEfficiency).equals(1.).help("Proportion of intensity actually emitted").unit("[0;1]");
    Input(minPeriodOn).help("Minimum period that light stays on").unit("m");
    Input(timeStep).imports("calendar[timeStepSecs]").unit("s");
    Output(periodOn).help("Time since last time light went on").unit("m");
    Output(currentlyOn).help("Light remains on for the minimum period").unit("y|n");;
}

void GrowthLight::initialize() {
    swReflectivityBottom   = swReflectivityTop =
    lwReflectivityBottom   = lwReflectivityTop = 0.;
    swTransmissivityBottom = lwTransmissivityTop = 1.;
    updateAbsorptivities();
}

void GrowthLight::reset() {
    noLight();
}

void GrowthLight::update() {
    currentlyOn = on || ( currentlyOn && (periodOn < minPeriodOn) );
    if (currentlyOn) {
        periodOn += timeStep/60.;
        parFluxDown = intensity*parPhotonCoef;
        swFluxDown  = intensity*(1. - propLw);
        lwFluxDown  = intensity*propLw;
    }
    else
        noLight();
}

void GrowthLight::noLight() {
    periodOn = 0.;
    currentlyOn = false;
    parFluxDown = swFluxDown = lwFluxDown = 0.;
}

} //namespace

