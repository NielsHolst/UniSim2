/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include "general.h"
#include <base/publish.h>
#include "energy_flux_sunlight.h"

using namespace base;

namespace vg {

PUBLISH(EnergyFluxSunlight)

EnergyFluxSunlight::EnergyFluxSunlight(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    help("computes energy flux from unabsorbed indoors light");
    // This is the amount of light energy (mostly sunlight) assumed to be turned into
    // heating of the air immediately
    Input(unabsorbedRadiation).imports("radiationAbsorbed[reflected]").unit("W/m2");
}

void EnergyFluxSunlight::update() {
    value = unabsorbedRadiation;
}

} //namespace

