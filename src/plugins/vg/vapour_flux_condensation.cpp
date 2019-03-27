/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "vapour_flux_condensation.h"

using std::max;
using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(VapourFluxCondensation)

VapourFluxCondensation::VapourFluxCondensation(QString name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    help("models vapour flux through condensation on inside greenhouse surface");
    Input(surfaceAreaPerGroundArea).help("Surface area per ground area").unit("m2/m2");
    Input(surfaceTemperature).imports("indoors/temperature[value]").unit("oC");
    Input(indoorsTemperature).imports("indoors/temperature[value]").unit("oC");
    Input(indoorsAh).imports("indoors/humidity[ah]").unit("kg/m3");
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

