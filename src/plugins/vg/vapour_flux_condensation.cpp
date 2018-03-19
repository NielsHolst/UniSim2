/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/

/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "vapour_flux_condensation.h"
#include "general.h"
#include <base/publish.h>

using namespace base;
using std::max;

namespace vg {

PUBLISH(VapourFluxCondensation)

VapourFluxCondensation::VapourFluxCondensation(QString name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    help("models vapour flux through condensation on inside greenhouse surface");
    Input(surfaceAreaPerGroundArea).help("Surface area per ground area [m2/m2]");
    Input(surfaceTemperature).imports("indoors/temperature[value]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(indoorsAh).imports("indoors/humidity[ah]");
}

void VapourFluxCondensation::update() {
    double dTemp = indoorsTemperature - surfaceTemperature;
    conductance = dTemp > 0
                  ? surfaceAreaPerGroundArea*1.64e-3*pow(dTemp, 1/3.) // GCC, p.147
                  : 0.;
    double surfaceSah = sah(surfaceTemperature);
    vapourFlux = max(conductance*(indoorsAh - surfaceSah), 0.);
    gain = conductance*surfaceSah;
}

} //namespace

