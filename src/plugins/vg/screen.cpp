/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "screen.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(Screen)

Screen::Screen(QString name, QObject *parent)
    : RadiationLayer(name, parent)
{
    help("models the impact on light and air fluxes of a screen");
    Input(haze).equals(0.8).help("Proportion of direct light transmitted as diffuse light [0;1]").unit("[0;1]");
    Input(transmissivityAir).equals(0.37).help("Air transmissivity when fully drawn [0;1]").unit("[0;1]");
    Input(transmissivityAirExponent).equals(4.).help("Non-linearity of increase in air transmissivity when screen is not fully drawn").unit("-");
    Input(state).equals(0.).help("Proportion drawn (0=fully withdrawn; 1=fully drawn").unit("[0;1]");

    Output(layer).help("Layer counting from nearest the cover; layered in the order of declaration").unit("[0,1,2,...]");
    Output(unhazed).help("Proportion of direct light transmitted as direct light, corrected for screen state [0;1]").unit("[0;1]");
    Output(heatCapacity).help("Whole-screen heat capacity").unit("J/K");
    Output(transmissivityAirNet).help("Air transmissivity, corrected for screen state").unit("[0;1]");
    Output(effectiveArea).help("Screen area, corrected for screen state").unit("m2");
}

void Screen::initialize() {
    // Default to an energy screen
    swReflectivityTop    = lwReflectivityTop      = 0.7;
    swReflectivityBottom = lwReflectivityBottom   = 0.09;
    swTransmissivityTop  = lwTransmissivityBottom = 0.01;
    Utop = Ubottom =  1.247;
    specificHeatCapacity = 2313.;
    temperature = 20.;
    updateAbsorptivities();
    // Set layer
    QVector<Screen*> screens = findMany<Screen>("AllSiblings::*<Screen>");
    layer = screens.indexOf(this);
    if (layer == -1)
        ThrowException("Unexpected: Cannot find self among among screens").context(this);
}

void Screen::reset() {
    updateByState(0);
}

void Screen::update() {
    updateByState(state);
}

void Screen::updateByState(double state) {
    if (state<0. || state>1.)
        ThrowException("Screen state out of [0;1] bounds").value(state).context(this);
    unhazed =  1. - state*haze;
    double state2 = 1. - pow(1.-state, transmissivityAirExponent);
    transmissivityAirNet = transmissivityAir + (1.-state2)*(1.-transmissivityAir);
    effectiveArea = area*state;
    heatCapacity = specificHeatCapacity*effectiveArea;
}

} //namespace

