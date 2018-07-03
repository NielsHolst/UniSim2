/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "general.h"
#include "screen.h"

using namespace base;

namespace vg {

PUBLISH(Screen)

Screen::Screen(QString name, QObject *parent)
    : Box(name, parent)
{
    help("models the impact on light and air fluxes of a screen");
    Input(shelterArea).imports("../..[area]");
    Input(orientation).equals("horizontal").help("Either 'cover' (i.e. along cover) or 'horizontal'");
    Input(transmissivityLight).equals(0.41).help("Transmissivity to short-wave radiation [0;1]");
    Input(emissivityInner).equals(0.62).help("Inner emissivity(=absorptivity) of long-wave radiation [0;1]");
    Input(emissivityOuter).equals(0.06).help("Outer emissivity(=absorptivity) of long-wave radiation [0;1]");
    Input(U).equals(2.).help("Heat transfer coefficient (W/m2/K)");
    Input(energyLossReduction).equals(0.6).help("Manufacturer's assessment of reduction in energy loss [0;1]");
    Input(haze).equals(0.8).help("Proportion of direct light transmitted as diffuse light [0;1]");
    Input(specificHeatCapacity).equals(1500.).help("Area-specific heat capacity (J/m2 cover/K)");
    Input(transmissivityAir).equals(0.37).help("Air transmissivity when fully drawn [0;1]");
    Input(transmissivityAirExponent).equals(4.).help("Non-linearity of increase in air transmissivity when screen is not fully drawn (-)");
    Input(state).equals(0.).help("Proportion drawn (0=fully withdrawn; 1=fully drawn");

    Output(layer).help("Layer (0,1,2...) counting from nearest the cover; layered in the order of declaration");
    Output(transmissivityLightNet).help("Transmissivity to short-wave radiation, corrected for screen state [0;1]");
    Output(absorptivityLwInnerNet).help("Inner emissivity(=absorptivity) to long-wave radiation, corrected for screen state [0;1]");;
    Output(absorptivityLwOuterNet).help("Outer emissivity(=absorptivity) to long-wave radiation, corrected for screen state [0;1]");;
    Output(unhazed).help("Proportion of direct light transmitted as direct light, corrected for screen state [0;1]");
    Output(resistance).help("The inverse the U-value, corrected for state (K m2/J)");
    Output(heatCapacity).help("Whole-screen heat capacity (J/K)");
    Output(transmissivityAirNet).help("Air transmissivity, corrected for screen state [0;1]");
    Output(effectiveArea).help("Screen area, corrected for screen state (m2)");
    Output(isHorizontal).help("Is the screen horizontally oriented?");
}

void Screen::initialize() {
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
    // Check values
    if (U<=0.)
        ThrowException("U-value must be positive").value(U).context(this);
}

void Screen::reset() {
    updateByState(0);
//    double slope = -U50/50.;
//    U = -100*slope + slope*energyLossReduction;
}

void Screen::update() {
    updateByState(state);
}

inline double diff1(double tr, double ab) {
    return 1. - tr - ab;
}

void Screen::updateByState(double state) {
    transmissivityLightNet= 1. - state + state*transmissivityLight;
    absorptivityLwInnerNet = state*emissivityInner;   // Absorptivity = Emissivity for IR
    absorptivityLwOuterNet = state*emissivityOuter;   // do.
    unhazed =  1. - state*haze;
    double state2 = 1. - pow(1.-state, transmissivityAirExponent);
    transmissivityAirNet = transmissivityAir + (1.-state2)*(1.-transmissivityAir);
    resistance = state/U;
    effectiveArea = shelterArea*state;
    heatCapacity = specificHeatCapacity*effectiveArea;
}

} //namespace

