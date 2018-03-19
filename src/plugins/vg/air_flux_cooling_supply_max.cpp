/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/publish.h>
#include "air_flux_cooling_supply_max.h"
#include "general.h"

using std::min;
using std::max;
using namespace base;

namespace vg {

PUBLISH(AirFluxCoolingSupplyMax)

AirFluxCoolingSupplyMax::AirFluxCoolingSupplyMax(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes max. supply of air flux");
    Input(byWind).imports("./byWind[max]");
    Input(byTemperature).imports("./byTemperature[max]");
    Input(givenAirflux).imports("given/airFlux[value]");
    Output(value).help("Max. relative rate of air exchange [m3/m3/h]");
}

void AirFluxCoolingSupplyMax::reset() {
    value = 0.;
}

void AirFluxCoolingSupplyMax::update() {
    value = max(sqrt(sqr(byWind) + sqr(byTemperature)) - givenAirflux, 0.);
}

} //namespace

