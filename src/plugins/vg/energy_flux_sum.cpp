/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "energy_flux_sum.h"
#include "general.h"
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(EnergyFluxSum)

EnergyFluxSum::EnergyFluxSum(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    help("computes the sum of all children's value port");
}

void EnergyFluxSum::initialize() {
    fluxes.clear();
    QVector<Port*> ports = Path("./*[value]", this).resolveMany<Port>(this);
    for (Port *port : ports)
        fluxes << port->valuePtr<double>();;
}

void EnergyFluxSum::update() {
    value = 0;
    for (auto flux : fluxes)
        value += *flux;
}

} //namespace

