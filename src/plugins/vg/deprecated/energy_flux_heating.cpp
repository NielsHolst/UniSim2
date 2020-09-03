/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "energy_flux_heating.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(EnergyFluxHeating)

EnergyFluxHeating::EnergyFluxHeating(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    Input(energyFluxes).imports("actuators/heating/pipes/*[energyFlux]", CA).help("Energy flux from each pipe");
}

void EnergyFluxHeating::reset() {
    update();
}

void EnergyFluxHeating::update() {
    value = vector_op::sum(energyFluxes);
}


} //namespace

