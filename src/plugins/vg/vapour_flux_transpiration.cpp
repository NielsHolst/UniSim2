/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "vapour_flux_transpiration.h"

using namespace base;

namespace vg {

PUBLISH(VapourFluxTranspiration)

VapourFluxTranspiration::VapourFluxTranspiration(QString name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    help("computes vapour flux from transpiration");
    Input(conductanceIn).imports("crop/conductance[value]",CA).unit("s/m");
    Input(vapourFluxIn).imports("crop/vapourFlux[value]",CA).unit("kg/m2/s");
    Input(gainIn).imports("crop/gain[value]",CA).unit("kg/m2/s");
}

void VapourFluxTranspiration::update() {
    conductance = conductanceIn;
    vapourFlux = vapourFluxIn;
    gain = gainIn;
}


} //namespace

