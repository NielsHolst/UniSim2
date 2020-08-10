/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include "heat_transfer_layer_base.h"

#include <base/dialog.h>

using namespace base;
using namespace phys_math;

namespace vg {

HeatTransferLayerBase::HeatTransferLayerBase(QString name, QObject *parent)
    : HeatTransferLayerParameters(name, parent)
{
    help("holds the radiative parameters of a layer, e.g., cover or screen");
    Input(parFluxDown).unit("umol/s/m2").help("PAR flux going down from bottom");
    Input(parFluxUp).unit("umol/s/m2").help("PAR flux going up from top");
    Input(swFluxDown).unit("W/m2").help("Short-wave flux going down from bottom");
    Input(swFluxUp).unit("W/m2").help("Short-wave flux going up from top");

    Input(convectiveInflux).help("Energy influx by convection").unit("W/m2");
    Input(conductiveInflux).help("Energy influx by conduction").unit("W/m2");
    Input(condensationRate).help("Water vapour condensation on layer").unit("kg/m2/s");
    Input(timeStep).imports("calendar[timeStepSecs]");

    Output(parAbsorbedFromAbove).unit("umol/s/m2").help("PAR flux absorbed from above");
    Output(parAbsorbedFromBelow).unit("umol/s/m2").help("PAR flux absorbed from below");
    Output(parAbsorbed).unit("umol/s/m2").help("PAR flux absorbed in total");
    Output(swAbsorbedFromAbove).unit("W/m2").help("Short-wave flux absorbed from above");
    Output(swAbsorbedFromBelow).unit("W/m2").help("Short-wave flux absorbed from below");
    Output(swAbsorbed).unit("W/m2").help("Short-wave flux absorbed in total");
    Output(lwAbsorbedFromAbove).unit("W/m2").help("Long-wave flux absorbed from above");
    Output(lwAbsorbedFromBelow).unit("W/m2").help("Long-wave flux absorbed from below");
    Output(lwAbsorbed).unit("W/m2").help("Long-wave flux absorbed in total ");
    Output(lwFluxDown).unit("W/m2").help("Long-wave flux going down from bottom");
    Output(lwFluxUp).unit("W/m2").help("Long-wave flux going up from");
    Output(absorbed).unit("W/m2").help("All flux (short+long) absorbed in total");
    Output(temperature).unit("oC").help("Surface temperature");
}


void HeatTransferLayerBase::updateLwEmission() {
    double lwFlux = Sigma*p4K(temperature);
    lwFluxDown = lwAbsorptivityBottom*lwFlux;
    lwFluxUp   = lwAbsorptivityTop*lwFlux;
}

void HeatTransferLayerBase::updateTemperature() {
    // K = W/m2 / (J/K/m2) * s = W/m2 * K*m2/J * s = K

//    dialog().information(QString("A HeatTransferLayerBase::updateTemperature %5 T=%1 abs=%2 conv=%3 cond=%4")
//                         .arg(temperature).arg(absorbed).arg(convectiveInflux).arg(conductiveInflux).arg(name()));
    double latentHeat = condensationRate*LHe; // W/m2 = kg/m2/s * J/kg
    temperature += (absorbed + convectiveInflux + conductiveInflux + latentHeat - lwFluxDown - lwFluxUp)/specificHeatCapacity*timeStep;
//    dialog().information(QString("B HeatTransferLayerBase::updateTemperature T=%1").arg(temperature));
}

} //namespace

