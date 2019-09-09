/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "energy_flux_condensation.h"
#include "general.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(EnergyFluxCondensation)

EnergyFluxCondensation::EnergyFluxCondensation(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    help("models energy flux caused by condensation");
    Input(vapourFlux).equals(0).help("Condensation rate per greenhouse ground area").unit("kg/m2/s");
}

void EnergyFluxCondensation::update() {
    value = vapourFlux*LHe; // W/m2 = kg/m2/s * J/kg
}

} //namespace

