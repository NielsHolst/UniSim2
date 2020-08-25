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
#include "heat_transfer_layer_base.h"

#include <base/dialog.h>

using namespace base;
using namespace phys_math;

namespace vg {

HeatTransferLayerBase::HeatTransferLayerBase(QString name, QObject *parent)
    : HeatTransferLayerParameters(name, parent)
{
    help("holds the radiative parameters of a layer, e.g., cover or screen");
    // All m2 are m2 ground
    Input(parFluxDown).unit("umol/s/m2").help("PAR flux going down from bottom");
    Input(parFluxUp).unit("umol/s/m2").help("PAR flux going up from top");
    Input(swFluxDown).unit("W/m2").help("Short-wave flux going down from bottom");
    Input(swFluxUp).unit("W/m2").help("Short-wave flux going up from top");

    Input(convectiveInflux).help("Energy influx by convection").unit("W/m2");
    Input(conductiveInflux).help("Energy influx by conduction").unit("W/m2");
    Input(condensationRate).help("Water vapour condensation on layer").unit("kg/m2/s");
    Input(maxTemperatureRateOfChange).equals(100.).unit("K/min")
            .help("Max. rate of change in temperature; surplus goes to unusedInflux");
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
    Output(latentHeat).unit("W/m2").help("Latent heat from water condensation");
    Output(temperature).unit("oC").help("Surface temperature");
    Output(unusedInflux).unit("W/m2").help("Influx left over too keep within maxTemperatureRateOfChange");
}


void HeatTransferLayerBase::updateLwEmission() {
    double lwFlux = Sigma*p4K(temperature);
    // Assumet that emmissivity equals absorptivity
    lwFluxDown = lwAbsorptivityBottom*lwFlux;
    lwFluxUp   = lwAbsorptivityTop*lwFlux;
}

void HeatTransferLayerBase::updateTemperature() {
    // K = W/m2 / (J/K/m2) * s = W/m2 * K*m2/J * s = K
    latentHeat = -condensationRate*LHe; // W/m2 = kg/m2/s * J/kg

//    dialog().information(QString("A HeatTransferLayerBase::updateTemperature %1 T=%2 abs=%3 conv=%4 cond=%5 latent=%6 C=%7")
//                         .arg(name()).arg(temperature).arg(absorbed).arg(convectiveInflux).arg(conductiveInflux).arg(latentHeat)
//                         .arg(heatCapacity)
//                         );
    if (TestNum::neZero(heatCapacity)) {
        double maxRateOfChange = maxTemperatureRateOfChange/60., // K/s
               rateOfChange  = (absorbed + convectiveInflux + conductiveInflux + latentHeat - lwFluxDown - lwFluxUp)/heatCapacity;  // K/s
        if (rateOfChange > maxRateOfChange) {
            unusedInflux = (rateOfChange - maxRateOfChange)*heatCapacity;  // W/m2 = K/s * J/K/m2
            rateOfChange = maxRateOfChange;
        }
        else if (rateOfChange < -maxRateOfChange) {
            unusedInflux = (rateOfChange + maxRateOfChange)*heatCapacity;  // W/m2 = K/s * J/K/m2
            rateOfChange = -maxRateOfChange;
        }
        else {
            unusedInflux = 0.;
        }
        temperature += rateOfChange*timeStep;
    }
//    dialog().information(QString("B HeatTransferLayerBase::updateTemperature T=%1").arg(temperature));
}

// Sets temperature to newTemperature;
// returns the energyInflux to the layerr (W/m2 ground, positive or negative) needed for the temperature change
//double HeatTransferLayerBase::setTemperature(double newTemperature) {
//    double dT = newTemperature - temperature;
//    if (TestNum::eqZero(dT) || TestNum::eqZero(heatCapacity))
//        return 0.;
//    QString s("setTemperature %1: %2");
//    dialog().information(s.arg(name()).arg(dT));

//    double energyInflux = dT/heatCapacity/timeStep; // W/m2 = K / (K*m2/J) / s
//    temperature = newTemperature;
//    return energyInflux;
//}

} //namespace

