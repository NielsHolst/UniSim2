/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "energy_flux_heating_supply.h"
#include <base/publish.h>

using std::min;
using namespace base;

namespace vg {

PUBLISH(EnergyFluxHeatingSupply)

/*! \class EnergyFluxHeatingSupply
 * \brief Flux of energy supplied to heat the greenhouse
 * Inputs
 * ------
 * - _maxHeating_ is the maximum heating that can be supplied [W/m<SUP>2</SUP>]
 * - _demand_ is the heating needed [W/m<SUP>2</SUP>]
 *
 * Output
 * ------
 * - _value_ is the heating supplied [W/m<SUP>2</SUP>]
 */

EnergyFluxHeatingSupply::EnergyFluxHeatingSupply(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    Input(maxHeating, 10000.);
    Input(demand, "heating/demand[value]");
}

void EnergyFluxHeatingSupply::update() {
    value = min(demand, maxHeating);
}

} //namespace

