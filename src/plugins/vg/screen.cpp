/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/exception.h>
#include "general.h"
#include <base/publish.h>
#include "screen.h"

using namespace base;

namespace vg {

PUBLISH(Screen)

/*! \class Screen
 * \brief Models different kinds of screens (black-out, energy, shade)
 *
 * Inputs
 * ------
 * - _position_ is one of eight screen positions ["whole_roof", "flat_roof", "roof1", "roof2", "side1", "side2", "end1", "end2"]
 * - _layer_ is one of three layers ["inner", "mid", "outer"]
 * - _transmissivityLight_ is the proportion of light transmitted [0;1]
 * - _emissivityInner_ is the emissivity (=absorptivity) for thermal radiation on the inwards side [0;1]
 * - _emissivityOuter_ is the emissivity (=absorptivity)for thermal radiation on the outwards side [0;1]
 * - _U50_ is the U-value corresponding to an _energyLossReduction_ of 0.5 [W/m<SUP>2</SUP>/K]
 * - _energyLossReduction_ is the proportion of energy saved by the screen [0;1]
 * - _haze_ is the proportion of direct light becoming dispersed on passage through the material [0;1]
 * - _specificHeatCapacity_ is the area-specific heat capacity [J/m<SUP>2</SUP> cover/K]
 * - _transmissivityAir_ is the proportion of air transmitted through the material [0;1]
 * - _transmissivityAirExponent_ corrects for non-linearity of air transmissivity to _state_ [-]
 * - _state_ is the state of the screen [0;1], where 0=open and 1=drawn
 *
 * Outputs
 * ------
 * - _transmissivityLightNet_ is the proportion of light transmitted corrected for _state_ [0;1]
 * - _absorptivityLwInnerNet_ is the absorptivity (=emissivity) for thermal radiation on the inwards side corrected for _state_ [0;1]
 * - _absorptivityLwOuterNet_ is the absorptivity (=emissivity) for thermal radiation on the outwards side corrected for _state_ [0;1]
 * - _unhazed_ is the proportion of direct light passing remaining direct after passage corrected for _state_ [0;1]
 * - _resistance_ is the inverse the U-value corrected for _state_ [K m<SUP>2</SUP>/J]
 * - _heatCapacity_ is the heat capacity [J/K]
 * - _transmissivityAirNet_ is the proportion of air transmitted, non-linearly corrected for _state_ [0;1]
 * - _effectiveArea_ is the screen max area corrected for _state_ [m<SUP>2</SUP>]
 * - _isHorizontal_ is the screen in horizontal position ?
 */

Screen::Screen(QString name, QObject *parent)
    : Box(name, parent)
{
    help("models the impact on light and air fluxes of a screen");
    Input(shelterArea).imports("../..[area]");
    Input(orientation).equals("cover").help("Either 'cover' (i.e. along cover) or 'horizontal'");
    Input(transmissivityLight).equals(0.41).help("Transmissivity to short-wave radiation [0;1]");
    Input(emissivityInner).equals(0.62).help("Inner emissivity(=absorptivity) of long-wave radiation [0;1]");
    Input(emissivityOuter).equals(0.06).help("Outer emissivity(=absorptivity) of long-wave radiation [0;1]");
    Input(U).equals(infinity()).help("Heat transfer coefficient (W/m2/K)");
    Input(energyLossReduction).equals(0.4).help("Manufacturer's assessment of reduction in energy loss [0;1]");
    Input(haze).equals(0.).help("Proportion of direct light transmitted as diffuse light [0;1]");
    Input(specificHeatCapacity).equals(1500.).help("Area-specific heat capacity (J/m2 cover/K)");
    Input(transmissivityAir).equals(0.8).help("Air transmissivity when fully drawn [0;1]");
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

