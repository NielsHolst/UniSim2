/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include <base/publish.h>
#include "vapour_flux_transpiration.h"

using namespace base;

namespace vg {

PUBLISH(VapourFluxTranspiration)

/*! \class VapourFluxTranspiration
 * \brief Vapour flux transpiring from crop
 *
 * Inputs
 * ------
 * - _conductanceIn_ is the transpiration conductance [m/s]
 * - _vapourFluxIn_ is the transpiration rate [kg/m<SUP>2</SUP>/s]
 * - _gainIn_ is used to integrate over all water vapour fluxes (GCC, p. 148) [kg/m<SUP>2</SUP>/s]
*/

VapourFluxTranspiration::VapourFluxTranspiration(QString name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    Input(conductanceIn, "crop/conductance[value]");
    Input(vapourFluxIn, "crop/vapourFlux[value]");
    Input(gainIn, "crop/gain[value]");
}

void VapourFluxTranspiration::update() {
    conductance = conductanceIn;
    vapourFlux = vapourFluxIn;
    gain = gainIn;
}


} //namespace

