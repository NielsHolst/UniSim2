/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "fruit_factor.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(FruitFactor)

FruitFactor::FruitFactor(QString name, QObject *parent)
    : Box(name, parent)
{
    help("models density-dependent fruit set");
    Input(minDensity).equals(1.).help("Crop density at which factor is 1").unit("/m2");
    Input(maxValue).equals(1.).help("Max. value of factor").unit("-");
    Input(plantDensity).imports("../..[density]").unit("/m2");
    Output(value).help("Density-dependent fruit set factor").unit("-");
}

void FruitFactor::reset() {
    value = 1.;
}

void FruitFactor::update() {
    const double a = 16,
                 b = 1.2;
    value = (plantDensity <= minDensity) ? 1. :
            1 + (maxValue-1)*(1-exp(-a*pow(plantDensity-minDensity, b)));
}


} //namespace

