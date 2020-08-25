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
#include "heat_transfer_layer_parameters.h"


using namespace base;
using namespace phys_math;
using namespace TestNum;

namespace vg {

HeatTransferLayerParameters::HeatTransferLayerParameters(QString name, QObject *parent)
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

    Input(Utop).equals(infinity()).unit("W/K/m2 layer").help("Heat transfer coefficient at the top");
    Input(Ubottom).equals(infinity()).unit("W/K/m2 layer").help("Heat transfer coefficient at the bottom");

    Input(area).unit("m2 layer").help("Area of layer");
    Input(heatCapacity).help("Area-specific heat capacity").unit("J/K/m2 layer");

    Output(swAbsorptivityTop).unit("[0;1]").help("Short-wave absorptivity at the top");
    Output(swAbsorptivityBottom).unit("[0;1]").help("Short-wave absorptivity at the bottom");
    Output(lwAbsorptivityTop).unit("[0;1]").help("Long-wave absorptivity at the top");
    Output(lwAbsorptivityBottom).unit("[0;1]").help("Long-wave absorptivity at the bottom");
}

#define CHK(x) checkRange(x, #x)
#define CHKZ(x) checkNotZero(x, #x)

void HeatTransferLayerParameters::updateAbsorptivities() {
//    const double minAbsorptivity = 0.02;
    swAbsorptivityTop = 1. - swReflectivityTop - swTransmissivityTop;
    swAbsorptivityBottom  = 1. - swReflectivityBottom  - swTransmissivityBottom;
    lwAbsorptivityTop = 1. - lwReflectivityTop - lwTransmissivityTop;
    lwAbsorptivityBottom  = 1. - lwReflectivityBottom  - lwTransmissivityBottom;

    snapTo(swAbsorptivityTop, 0.); snapTo(swAbsorptivityTop, 1.);
    snapTo(swAbsorptivityBottom , 0.); snapTo(swAbsorptivityBottom , 1.);
    snapTo(lwAbsorptivityTop, 0.); snapTo(lwAbsorptivityTop, 1.);
    snapTo(lwAbsorptivityBottom , 0.); snapTo(lwAbsorptivityBottom , 1.);

//    if (TestNum::eqZero(swAbsorptivityTop)) {
//        swAbsorptivityTop = minAbsorptivity;
//        if (swReflectivityTop > swTransmissivityTop)
//            swReflectivityTop -= minAbsorptivity;
//        else
//            swTransmissivityTop -= minAbsorptivity;
//    }
//    if (TestNum::eqZero(swAbsorptivityBottom)) {
//        swAbsorptivityBottom = minAbsorptivity;
//        if (swReflectivityBottom > swTransmissivityBottom)
//            swReflectivityBottom -= minAbsorptivity;
//        else
//            swTransmissivityBottom -= minAbsorptivity;
//    }
//    if (TestNum::eqZero(lwAbsorptivityTop)) {
//        lwAbsorptivityTop = minAbsorptivity;
//        if (lwReflectivityTop > lwTransmissivityTop)
//            lwReflectivityTop -= minAbsorptivity;
//        else
//            lwTransmissivityTop -= minAbsorptivity;
//    }
//    if (TestNum::eqZero(lwAbsorptivityBottom)) {
//        lwAbsorptivityBottom = minAbsorptivity;
//        if (lwReflectivityBottom > lwTransmissivityBottom)
//            lwReflectivityBottom -= minAbsorptivity;
//        else
//            lwTransmissivityBottom -= minAbsorptivity;
//    }

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

//    CHKZ(swAbsorptivityTop);
//    CHKZ(swAbsorptivityBottom);
//    CHKZ(lwAbsorptivityTop);
//    CHKZ(lwAbsorptivityBottom);
}

void HeatTransferLayerParameters::checkRange(double x, QString name) const {
    if (TestNum::ltZero(x) || TestNum::gt(x, 1.))
        ThrowException("Radiative parameter must be inside [0;1]").
                value(name).value2(x).context(this);
}
void HeatTransferLayerParameters::checkNotZero(double x, QString name) const {
    if (TestNum::eqZero(x))
        ThrowException(name + " == 1. Reflectivity + Transmissivity must be < 1.").context(this);
}

} //namespace

