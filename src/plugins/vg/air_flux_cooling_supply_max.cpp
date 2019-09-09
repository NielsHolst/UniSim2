/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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
    : AirFluxBase(name, parent)
{
    help("computes max. supply of air flux");
    Input(byWind).imports("./byWind[max]");
    Input(byTemperature).imports("./byTemperature[max]");
    Input(givenAirflux).imports("given/airFlux[value]");
}

void AirFluxCoolingSupplyMax::update() {
    value = max(sqrt(sqr(byWind) + sqr(byTemperature)) - givenAirflux, 0.);
}

} //namespace

