/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "heat_transfer_volume.h"
#include "heat_transfer_layer_base.h"

#include <base/dialog.h>

using namespace base;
using namespace phys_math;

//#define LOG(x) dialog().information(x);
#define LOG(x)

namespace vg {

PUBLISH(HeatTransferVolume)

HeatTransferVolume::HeatTransferVolume(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initTemperature).equals(20.).help("Initial air temperature").unit("oC");
    Input(convectiveInflux).help("Energy influx by convection").unit("W/m2 ground");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(groundArea).imports("geometry[groundArea]", CA);
    Input(volume).imports("geometry[volume]", CA);
    Input(airInflux).imports("indoors/ventilation[absolute]", CA).unit("m3");
    Input(outdoorsTemperature).imports("outdoors[temperature]", CA);
    Input(sumUnusedInfluxes).imports("othersiblings::*[unusedInflux]").transform(Sum);
    Input(maxTemperatureRateOfChange).equals(2.).unit("K/min")
            .help("Max. rate of change in temperature; surplus goes to heatBuffer");
    Output(heatBuffer).unit("W/m2").help("Buffer of unused heat input");
    Output(temperature).help("Current air temperature").unit("oC");
    Output(advectiveEnergyFlux).help("Energy exchanged with outdoors by air exchange").unit("W/m2");
}

void HeatTransferVolume::reset() {
    temperature = initTemperature;
    screens.clear();
    for (int i = 1; i<=3; ++i) {
        HeatTransferLayerBase *scr = findScreen(i);
        if (scr)
            screens << scr;
    }
}

HeatTransferLayerBase* HeatTransferVolume::findScreen(int i) {
    return findMaybeOne<HeatTransferLayerBase>("../screen" + QString::number(i));
}

void HeatTransferVolume::update() {
    // Compute temperature by air exchange in terms of energy flux
    // W/m2 = K * J/kg/K * kg/m3 *m3 /m2 /s
    advectiveEnergyFlux = (outdoorsTemperature-temperature)*CpAir*RhoAir*airInflux/groundArea/timeStep;

    QString s(
                "heatCapacity: %1"
                "\nconvectiveInflux: %2"
                "\nadvectiveEnergyFlux: %3"
                "\nheatBuffer: %4"
                "\n-sumUnusedInfluxes: %5"
                "\nrateOfChange: %6"
                "\nmaxRateOfChange: %7"
              );

    double maxRateOfChange = maxTemperatureRateOfChange/60., // K/s
           mass = RhoAir*volume,  // kg = kg/m3 * m3
           heatCapacity = CpAir*mass/groundArea,  // J/K/m2 = (J/kg/K) * kg / m2
           heatInflux = convectiveInflux + advectiveEnergyFlux + heatBuffer + sumUnusedInfluxes, // W/m2
           rateOfChange  = heatInflux/heatCapacity;  // K/s
    LOG("A " + s.arg(heatCapacity).arg(convectiveInflux).arg(advectiveEnergyFlux).arg(heatBuffer)
        .arg(-sumUnusedInfluxes).arg(rateOfChange).arg(maxRateOfChange))
    if (rateOfChange > maxRateOfChange) {
        heatBuffer = (rateOfChange - maxRateOfChange)*heatCapacity;  // W/m2 = K/s * J/K/m2
        rateOfChange = maxRateOfChange;
    }
    else if (rateOfChange < -maxRateOfChange) {
        heatBuffer = (rateOfChange + maxRateOfChange)*heatCapacity;  // W/m2 = K/s * J/K/m2
        rateOfChange = -maxRateOfChange;
    }
    else {
        heatBuffer = 0.;
    }
    temperature += rateOfChange*timeStep;
    LOG("Z " + s.arg(heatCapacity).arg(convectiveInflux).arg(advectiveEnergyFlux).arg(heatBuffer)
        .arg(-sumUnusedInfluxes).arg(rateOfChange).arg(maxRateOfChange))
    if (fabs(heatBuffer) > 1e5)
            ThrowException("Model is numerically unstable. Shorten the time step.").context(this);
}

} //namespace

