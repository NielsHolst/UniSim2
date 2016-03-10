/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_sum.h"
#include "general.h"
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(EnergyFluxSum)

/*! \class EnergyFluxSum
 * \brief Sum of child model energy fluxes
 *
 * Output
 * ------
 * - _value_ is the sum of all children's _value_s [W/m<SUP>2</SUP>]
 *
 * Inputs
 * ------
 * - all EnergyFluxBase child models
 */

EnergyFluxSum::EnergyFluxSum(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
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

