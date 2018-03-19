/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
    Class(EnergyFluxBase);
    help("is a base class for models of energy flux to the greenhouse air");
    Output(value).help("Energy flux to the greenhouse air per greenhouse area [W/m2");
}


} //namespace

