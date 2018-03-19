/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/

#include "boundary_layer_resistance_base.h"
#include <base/publish.h>

using namespace base;

namespace vg {

BoundaryLayerResistanceBase::BoundaryLayerResistanceBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(BoundaryLayerResistanceBase);
    help("base class to compute bounday layer resistance");
    Input(leafDimension).equals(25./1000.).help("Description of leaf geometry [mm]");
    Input(leafWindSpeed).imports("../windSpeed[value]");
    Output(rbH2O).help("The boundary layer resistance against water vapour [s/m]");
    Output(rbCo2).help("The boundary layer resistance against CO2 [s/m]");
}

void BoundaryLayerResistanceBase::reset() {
    update();
}

void BoundaryLayerResistanceBase::update() {
    setRbH2O();
    rbCo2 = rbH2O*1.37;
}

} //namespace
