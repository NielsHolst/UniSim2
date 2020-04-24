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
#include "radiation_layer.h"

using namespace base;
using namespace TestNum;
using namespace phys_math;

namespace vg {

RadiationLayer::RadiationLayer(QString name, QObject *parent)
    : Box(name, parent)
{
    help("holds the radiative parameters of a layer, e.g., cover or screen");
    Input(swReflectivityTop).unit("[0;1]").help("Short-wave reflectivity at the top");
    Input(swReflectivityBottom).unit("[0;1]").help("Short-wave reflectivity at the bottom");
    Input(swTransmissivityTop).equals(1.).unit("[0;1]").help("Short-wave transmissivity at the top");
    Input(swTransmissivityBottom).equals(1.).unit("[0;1]").help("Short-wave transmissivity at the bottom");
    Input(lwReflectivityTop).unit("[0;1]").help("Long-wave reflectivity at the top");
    Input(lwReflectivityBottom).unit("[0;1]").help("Long-wave reflectivity at the bottom");
    Input(lwTransmissivityTop).equals(1.).unit("[0;1]").help("Long-wave transmissivity at the top");
    Input(lwTransmissivityBottom).equals(1.).unit("[0;1]").help("Long-wave transmissivity at the bottom");

    Input(parFluxDown).unit("umol/s/m2").help("PAR flux going down from bottom");
    Input(parFluxUp).unit("umol/s/m2").help("PAR flux going up from top");
    Input(swFluxDown).unit("W/m2").help("Short-wave flux going down from bottom");
    Input(swFluxUp).unit("W/m2").help("Short-wave flux going up from top");
    Input(lwFluxDown).unit("W/m2").help("Long-wave flux going down from bottom");
    Input(lwFluxUp).unit("W/m2").help("Long-wave flux going up from");

    Input(Utop).equals(infinity()).unit("W/m2/K").help("Heat transfer coefficient at the top");
    Input(Ubottom).equals(infinity()).unit("W/m2/K").help("Heat transfer coefficient at the bottom");

    Input(area).imports("ancestors::*[area]",CA).unit("m2 surface").help("Surface area");
    Input(specificHeatCapacity).equals(infinity()).help("Area-specific heat capacity").unit("J/K/m2 surface");

    Output(swAbsorptivityTop).unit("[0;1]").help("Short-wave absorptivity at the top");
    Output(swAbsorptivityBottom).unit("[0;1]").help("Short-wave absorptivity at the bottom");
    Output(lwAbsorptivityTop).unit("[0;1]").help("Long-wave absorptivity at the top");
    Output(lwAbsorptivityBottom).unit("[0;1]").help("Long-wave absorptivity at the bottom");

    Output(parAbsorbedFromAbove).unit("umol/s/m2").help("PAR flux absorbed from above");
    Output(parAbsorbedFromBelow).unit("umol/s/m2").help("PAR flux absorbed from below");
    Output(parAbsorbed).unit("umol/s/m2").help("PAR flux absorbed in total");
    Output(swAbsorbedFromAbove).unit("W/m2").help("Short-wave flux absorbed from above");
    Output(swAbsorbedFromBelow).unit("W/m2").help("Short-wave flux absorbed from below");
    Output(swAbsorbed).unit("W/m2").help("Short-wave flux absorbed in total");
    Output(lwAbsorbedFromAbove).unit("W/m2").help("Long-wave flux absorbed from above");
    Output(lwAbsorbedFromBelow).unit("W/m2").help("Long-wave flux absorbed from below");
    Output(lwAbsorbed).unit("W/m2").help("Long-wave flux absorbed in total ");
    Output(absorbed).unit("W/m2").help("All flux (short+long) absorbed in total");
    Output(temperature).unit("oC").help("Surface temperature");
}

#define CHK(x) checkRange(x, #x)

void RadiationLayer::updateAbsorptivities() {
    swAbsorptivityTop = 1. - swReflectivityTop - swTransmissivityTop;
    swAbsorptivityBottom  = 1. - swReflectivityBottom  - swTransmissivityBottom;
    lwAbsorptivityTop = 1. - lwReflectivityTop - lwTransmissivityTop;
    lwAbsorptivityBottom  = 1. - lwReflectivityBottom  - lwTransmissivityBottom;
//    snapTo(swAbsorptivityTop, 0.); snapTo(swAbsorptivityTop, 1.);
//    snapTo(swAbsorptivityBottom , 0.); snapTo(swAbsorptivityBottom , 1.);
//    snapTo(lwAbsorptivityTop, 0.); snapTo(lwAbsorptivityTop, 1.);
//    snapTo(lwAbsorptivityBottom , 0.); snapTo(lwAbsorptivityBottom , 1.);
    CHK(swReflectivityTop);
    CHK(swReflectivityBottom);
    CHK(swTransmissivityTop);
    CHK(swTransmissivityBottom);
    CHK(lwReflectivityTop);
    CHK(lwReflectivityBottom);
    CHK(lwTransmissivityTop);
    CHK(lwTransmissivityBottom);
    CHK(swAbsorptivityTop);
    CHK(swAbsorptivityBottom);
    CHK(lwAbsorptivityTop);
    CHK(lwAbsorptivityBottom);
}

void RadiationLayer::checkRange(double x, QString name) const {
    if (TestNum::ltZero(x) || TestNum::gt(x, 1.))
        ThrowException("Radiative parameter must be inside [0;1]").
                value(name).value2(x).context(this);
}

void RadiationLayer::updateLwFluxes() {
    if (temperature > T0) {
        double lwFlux = 5.6704e-8*p4(temperature);
        lwFluxDown = lwAbsorptivityBottom*lwFlux;
        lwFluxUp   = lwAbsorptivityTop*lwFlux;
    }
}

void RadiationLayer::updateTemperature(double timeStep) {
    temperature += (absorbed + convectiveInflux + conductiveInflux)/specificHeatCapacity*timeStep;  // area?
}

} //namespace

