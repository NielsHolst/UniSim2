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
#include "energy_flux_air.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(EnergyFluxAir)

EnergyFluxAir::EnergyFluxAir(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    help("models energy flux carried by air from outdoors to indoors");
    Input(airFlux).equals(0).help("Rate of air exchange between outdoors and indoors").unit("m3/m3/h");
    Input(indoorsTemperature).imports("indoors/temperature[value]",CA).unit("oC");
    Input(outdoorsTemperature).imports("outdoors[temperature]",CA).unit("oC");
    Input(height).imports("geometry[indoorsAverageHeight]",CA).unit("m");
    Input(timeStepSecs).imports("calendar[timeStepSecs]").unit("s");
}

inline double y(double y0, double z, double v, double dt) {
  return z + (y0 - z)*exp(-v*dt);
}

void EnergyFluxAir::update() {
    double indoorsTemperatureFinal = y(indoorsTemperature, outdoorsTemperature, airFlux, timeStepSecs/3600.),
           dT = indoorsTemperatureFinal - indoorsTemperature,
           Cair = height*CpAirVol;   // J/K/m2 = m * J/K/m3
    value = Cair*dT*airFlux/3600;    // W/m2 = J/K/m2 / h * h/s
}

} //namespace

