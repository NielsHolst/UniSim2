/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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
    Input(leafDimension).equals(25./1000.).help("Description of leaf geometry").unit("mm");
    Input(leafWindSpeed).imports("indoors/windSpeed[value]",CA).unit("m/s");
    Output(rbH2O).help("The boundary layer resistance against water vapour").unit("s/m");
    Output(rbCo2).help("The boundary layer resistance against CO2").unit("s/m");
}

void BoundaryLayerResistanceBase::reset() {
    update();
}

void BoundaryLayerResistanceBase::update() {
    if (leafWindSpeed <=0. )
        ThrowException("Leaf wind speed must be > 0").value(leafWindSpeed).context(this);
    setRbH2O();
    rbCo2 = rbH2O*1.37;
}

} //namespace
