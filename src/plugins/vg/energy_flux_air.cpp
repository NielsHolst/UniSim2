/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include "general.h"
#include <base/publish.h>
#include "energy_flux_air.h"

using namespace base;

namespace vg {

PUBLISH(EnergyFluxAir)

EnergyFluxAir::EnergyFluxAir(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    help("models energy flux carried by air from outdoors to indoors");
    Input(airFlux).equals(0).help("Rate of air exchange between outdoors and indoors [m3/m3/h]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(height).imports("geometry[indoorsAverageHeight]");
}

void EnergyFluxAir::update() {
    double Cair = height*CpAirVol,   // J/K/m2 = m * J/K/m3
           dT = outdoorsTemperature - indoorsTemperature;
    value = Cair*dT*airFlux/3600;    // W/m2 = J/K/m2 / h * h/s
}

} //namespace

