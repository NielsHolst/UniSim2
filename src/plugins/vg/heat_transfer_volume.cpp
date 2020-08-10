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

#include <base/dialog.h>

using namespace base;
using namespace phys_math;

#define LOG(x)
//dialog().information(x)

namespace vg {

PUBLISH(HeatTransferVolume)

HeatTransferVolume::HeatTransferVolume(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initTemperature).equals(20.).help("Initial air temperature").unit("oC");
    Input(convectiveInflux).help("Energy influx by convection").unit("W");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(groundArea).imports("geometry[groundArea], CA");
    Input(volume).help("Air space volume").unit("m3");
    Input(outdoorsInfluxVolume).help("Influx of air from outdoors").unit("m3");
    Input(outdoorsTemperature).imports("outdoors[temperature]", CA);
    Input(neighbourInfluxVolume).help("Influx of air from neighbour").unit("m3");
    Input(neighbourTemperature).help("Temperature of air from neighbour").unit("oC");
    Output(temperature).help("Current air temperature").unit("oC");
    Output(advectiveEnergyFlux).help("Energy exchanged with outdoors by air exchange").unit("W/m2");
}

void HeatTransferVolume::reset() {
    temperature = initTemperature;
}

void HeatTransferVolume::update() {
    if (volume==0.) {
        advectiveEnergyFlux = 0.;
    }
    else {
        // Update temperature by convection
        double mass = RhoAir*volume;  // kg = kg/m3 * m3
        temperature += convectiveInflux/CpAir/mass*timeStep; // K = W / (J/kg/K) / kg * s
        // Update temperature by influx from outdoors and neighbour compartments
        double prevTemperature = temperature;
        updateByAdvection(neighbourInfluxVolume, neighbourTemperature);
        updateByAdvection(outdoorsInfluxVolume,  outdoorsTemperature);
        advectiveEnergyFlux = (temperature - prevTemperature)*CpAir*RhoAir*outdoorsInfluxVolume/groundArea/timeStep;
        // W/m2 = K * J/kg/K * kg/m3 *m3 /m2 /s
    }
}

void HeatTransferVolume::updateByAdvection(double influxVolume, double influxTemperature) {
    TestNum::snapTo(influxVolume, volume);
    if (influxVolume> volume)
        ThrowException("Influx cannot be larger than volume").value(influxVolume).value2(volume).context(this);
    double dV = influxVolume/volume;
    temperature = dV*influxTemperature + (1.-dV)*temperature;
}

} //namespace

