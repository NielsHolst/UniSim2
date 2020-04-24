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
    Input(orientation).equals("horizontal").help("Either 'cover' (i.e. along cover) or 'horizontal'").unit("m");
    Input(haze).equals(0.8).help("Proportion of direct light transmitted as diffuse light [0;1]").unit("[0;1]");
    Input(transmissivityAir).equals(0.37).help("Air transmissivity when fully drawn [0;1]").unit("[0;1]");
    Input(transmissivityAirExponent).equals(4.).help("Non-linearity of increase in air transmissivity when screen is not fully drawn").unit("-");
    Input(state).equals(0.).help("Proportion drawn (0=fully withdrawn; 1=fully drawn").unit("[0;1]");
    Input(airSpaceWidth).equals(0.1).help("Distance between screen and cover (or outer screen); zero means horizontal under roof");

    Output(layer).help("Layer counting from nearest the cover; layered in the order of declaration").unit("[0,1,2,...]");
    Output(unhazed).help("Proportion of direct light transmitted as direct light, corrected for screen state [0;1]").unit("[0;1]");
    Output(heatCapacity).help("Whole-screen heat capacity").unit("J/K");
    Output(transmissivityAirNet).help("Air transmissivity, corrected for screen state").unit("[0;1]");
    Output(effectiveArea).help("Screen area, corrected for screen state").unit("m2");
    Output(volumeBehindScreen).help("Volume behind screen area").unit("m2");;
    Output(isHorizontal).help("Is the screen horizontally oriented?").unit("y|n");

    // Default to an energy screen
    port("swReflectivityTop")->equals(0.7);
    port("swReflectivityBottom")->equals(0.09);
    port("swTransmissivity")->equals(0.01);

    port("lwReflectivityTop")->equals(0.7);
    port("lwReflectivityBottom")->equals(0.09);
    port("lwTransmissivity")->equals(0.01);

    port("Utop")->equals(1.247);
    port("Ubottom")->equals(1.247);
    port("specificHeatCapacity")->equals(2313.); // plastic
    port("temperature")->equals(20.); // initial value
}

void Screen::initialize() {
    updateAbsorptivities();
    // Set orientation
    if (orientation == "cover")
        _orientation = Cover;
    else if (orientation == "horizontal")
        _orientation = Horizontal;
    else
        ThrowException("Orientation must be 'cover' or 'horizontal'").value(orientation).context(this);
    isHorizontal = (_orientation == Horizontal);
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
    heatCapacity = specificHeatCapacity*area;
}

} //namespace

