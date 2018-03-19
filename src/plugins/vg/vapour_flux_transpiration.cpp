/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "general.h"
#include <base/publish.h>
#include "vapour_flux_transpiration.h"

using namespace base;

namespace vg {

PUBLISH(VapourFluxTranspiration)

VapourFluxTranspiration::VapourFluxTranspiration(QString name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    help("computes vapour flux from transpiration");
    Input(conductanceIn).imports("crop/conductance[value]");
    Input(vapourFluxIn).imports("crop/vapourFlux[value]");
    Input(gainIn).imports("crop/gain[value]");
}

void VapourFluxTranspiration::update() {
    conductance = conductanceIn;
    vapourFlux = vapourFluxIn;
    gain = gainIn;
}


} //namespace

