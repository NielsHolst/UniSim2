/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "boundary_layer_resistance_stanghellini.h"

using namespace std;
using namespace base;
using phys_math::sqr;

namespace vg {

PUBLISH(BoundaryLayerResistanceStanghellini)

BoundaryLayerResistanceStanghellini::BoundaryLayerResistanceStanghellini(QString name, QObject *parent)
    : BoundaryLayerResistanceBase(name, parent)
{
    help("computes boundary layer resistance from Stanghellini (1987, p. 32)");
    Input(leafTemperature).imports("../temperature[value]",CA).unit("oC");
    Input(indoorsTemperature).imports("indoors/temperature[value]",CA).unit("oC");
}

void BoundaryLayerResistanceStanghellini::setRbH2O() {
    double dTemp = fabs(leafTemperature - indoorsTemperature);
    rbH2O = 1174*sqrt(leafDimension)/pow(leafDimension*dTemp + 207*sqr(leafWindSpeed), 0.25);
}

} //namespace

