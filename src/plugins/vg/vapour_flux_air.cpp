/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "vapour_flux_air.h"

using namespace base;

namespace vg {

PUBLISH(VapourFluxAir)

VapourFluxAir::VapourFluxAir(QString name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    help("models vapour flux through air exchange");
    Input(airFlux).help("Rate of air exchange with outdoors").unit("m3/m3/h");
    Input(indoorsAh).imports("indoors/humidity[ah]",CA).unit("kg/m3");
    Input(outdoorsAh).imports("outdoors[ah]",CA).unit("kg/m3");
    Input(height).imports("geometry[indoorsAverageHeight]",CA).unit("m");
}

void VapourFluxAir::update() {
    conductance = height*airFlux/3600; // m/s = m3/m2 * m3/m3/h / (s/h)
    vapourFlux = conductance*(outdoorsAh-indoorsAh);    // kg/m2/s = m/s * kg/m3
    gain = conductance*outdoorsAh;                      // kg/m2/s = m/s * kg/m3
}

} //namespace

