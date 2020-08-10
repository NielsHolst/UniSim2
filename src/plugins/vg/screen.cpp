/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/test_num.h>
#include "screen.h"

using namespace base;
using namespace phys_math;

namespace vg {

Screen::Screen(QString name, QObject *parent)
    : HeatTransferLayerParameters(name, parent)
{
    help("models the impact on light and air fluxes of a screen");
    Input(haze).equals(1.).help("Proportion of direct light transmitted as diffuse light (not used)").unit("[0;1]");
    Input(transmissivityAir).equals(0.37).help("Air transmissivity when fully drawn").unit("[0;1]");
    Input(state).equals(0.).help("Proportion drawn (0=fully withdrawn; 1=fully drawn").unit("[0;1]");
    port("Utop")->equals(1.247);
    port("Ubottom")->equals(1.247);

    Output(maxArea).help("Max. area covered by screen").unit("m");
    Output(depth).help("Depth of volume covered by screen").unit("m");
    Output(heatCapacity).help("Heat capacity of drawn part of screen").unit("J/K");
    Output(swReflectivityTopNet).help("Parameter corrected for state").unit("[0;1]");
    Output(swReflectivityBottomNet).help("Parameter corrected for state").unit("[0;1]");
    Output(swTransmissivityTopNet).help("Parameter corrected for state").unit("[0;1]");
    Output(swTransmissivityBottomNet).help("Parameter corrected for state").unit("[0;1]");
    Output(lwReflectivityTopNet).help("Parameter corrected for state").unit("[0;1]");
    Output(lwReflectivityBottomNet).help("Parameter corrected for state").unit("[0;1]");
    Output(lwTransmissivityTopNet).help("Parameter corrected for state").unit("[0;1]");
    Output(lwTransmissivityBottomNet).help("Parameter corrected for state").unit("[0;1]");
    Output(swAbsorptivityTopNet).help("Parameter corrected for state").unit("[0;1]");
    Output(swAbsorptivityBottomNet).help("Parameter corrected for state").unit("[0;1]");
    Output(lwAbsorptivityTopNet).help("Parameter corrected for state").unit("[0;1]");
    Output(lwAbsorptivityBottomNet).help("Parameter corrected for state").unit("[0;1]");
}

void Screen::reset() {
    updateByState(0);
    maxArea = computeMaxArea();
    depth = computeDepth();
}

void Screen::update() {
    updateByState(state);
}

#define ADJUST_BY_STATE(x)    x##Net = state*x
#define ADJUST_BY_STATE_TR(x) x##Net = state*x + 1 - state

void Screen::updateByState(double state) {
    if (state<0. || state>1.)
        ThrowException("Screen state out of [0;1] bounds").value(state).context(this);
    area = maxArea*state;
    heatCapacity = specificHeatCapacity*area;
    updateAbsorptivities();
    ADJUST_BY_STATE(swReflectivityTop);
    ADJUST_BY_STATE(swReflectivityBottom);
    ADJUST_BY_STATE_TR(swTransmissivityTop);
    ADJUST_BY_STATE_TR(swTransmissivityBottom);
    ADJUST_BY_STATE(lwReflectivityTop);
    ADJUST_BY_STATE(lwReflectivityBottom);
    ADJUST_BY_STATE_TR(lwTransmissivityTop);
    ADJUST_BY_STATE_TR(lwTransmissivityBottom);
    ADJUST_BY_STATE(swAbsorptivityTop);
    ADJUST_BY_STATE(swAbsorptivityBottom);
    ADJUST_BY_STATE(lwAbsorptivityTop);
    ADJUST_BY_STATE(lwAbsorptivityBottom);
}

} //namespace

