/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "energy_flux_floor.h"
#include "general.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(EnergyFluxFloor)

EnergyFluxFloor::EnergyFluxFloor(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    help("models floor temperature and energy flux to greenhouse");
    Input(Uindoors).equals(7.5).unit("W/m2/K");
    Input(Usoil).equals(7.5).unit("W/m2/K");
    Input(heatCapacity).equals(42000.).help("Heat capacity of the floor").unit("J/m2/K");
    Input(emissivity).equals(0.85).help("Emissivity of thermal radiation from the floor").unit("[0;1]");  // concrete
    Input(indoorsTemperature).imports("indoors/temperature[value]").unit("oC");
    Input(soilTemperature).imports("outdoors[soilTemperature]").unit("oC");
    Input(height).imports("geometry[indoorsAverageHeight]").unit("m");
    Input(timeStep).imports("calendar[timeStepSecs]").unit("s");
    Output(temperature).help("Floor temperature").unit("oC");
}

void EnergyFluxFloor::reset() {
    temperature = 18.;
}

void EnergyFluxFloor::update() {
    const int maxTimeStep = 20;     // Use time steps no larger than this [s]
    int n = int(timeStep/maxTimeStep) + 1;
    double dt = timeStep/n,
           Tin = indoorsTemperature,
           Cair = height*RhoAir*CpAir;               // J/m2/K = m * kg/m3 * J/kg/K

    value = 0.;
    for (int i=0; i<n; ++i) {
        double fluxFloorToSoil = Usoil*(temperature-soilTemperature),
               fluxFloorToAir = Uindoors*(temperature-Tin);
        temperature -= (fluxFloorToSoil + fluxFloorToAir)*dt/heatCapacity;
        Tin += fluxFloorToAir*dt/Cair;
        value += fluxFloorToAir;
    }
    value /= n;
}

} //namespace

