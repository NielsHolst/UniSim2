/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "boundary_layer_resistance_stanghellini.h"
#include "general.h"
#include <base/publish.h>

using namespace std;
using namespace base;

namespace vg {

PUBLISH(BoundaryLayerResistanceStanghellini)

BoundaryLayerResistanceStanghellini::BoundaryLayerResistanceStanghellini(QString name, QObject *parent)
    : BoundaryLayerResistanceBase(name, parent)
{
    Class(BoundaryLayerResistanceStanghellini);
    help("computes boundary layer resistance from Stanghellini (1987, p. 32)");
    Input(leafTemperature).imports("../temperature[value]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
}

void BoundaryLayerResistanceStanghellini::setRbH2O() {
    double dTemp = fabs(leafTemperature - indoorsTemperature);
    rbH2O = 1174*sqrt(leafDimension)/pow(leafDimension*dTemp + 207*sqr(leafWindSpeed), 0.25);
}

} //namespace

