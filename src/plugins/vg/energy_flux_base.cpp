/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_base.h"
#include "general.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(EnergyFluxBase)

/*! \class EnergyFluxBase
 * \brief Base class for energy fluxes to ambient greenhouse air
 *
 * Output
 * ------
 * - _value_ is the energy flux to the greenhouse air per greenhouse area [W/m<SUP>2</SUP>]
 *
 */

EnergyFluxBase::EnergyFluxBase(QString name, QObject *parent)
	: Box(name, parent)
{
    Output(value);
}

void EnergyFluxBase::reset() {
    value = 0.;
}


} //namespace

