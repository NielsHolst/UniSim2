/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "energy_flux_transpiration.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(EnergyFluxTranspiration)

EnergyFluxTranspiration::EnergyFluxTranspiration(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    help("models energy flux caused by transpiration");
    Input(transpiration).imports("../../vapourFlux/transpiration[vapourFlux]").unit("kg/m2/s");
}

void EnergyFluxTranspiration::update() {
    value = -transpiration * LHe; // kg/m2/s * J/kg
}

} //namespace

