/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "vapour_flux_ventilation.h"

using namespace base;

namespace vg {

PUBLISH(VapourFluxVentilation)

VapourFluxVentilation::VapourFluxVentilation(QString name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    help("models vapour flux through air exchange");
    Input(ventilationTotal).imports("ventilation/total[value]",CA);
    Input(averageHeight).imports("geometry[averageHeight]",CA);
    Input(indoorsAh).imports("indoors/humidity[ah]",CA);
    Input(outdoorsAh).imports("outdoors[ah]",CA);
}

void VapourFluxVentilation::update() {
    conductance = averageHeight*ventilationTotal/3600.;  // m/s
    vapourFlux = conductance*(outdoorsAh-indoorsAh);     // kg/m2/s = m/s * kg/m3
    gain = conductance*outdoorsAh;                       // kg/m2/s = m/s * kg/m3
}

} //namespace

