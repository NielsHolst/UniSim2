/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "fruit_factor.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(FruitFactor)

/*! \class FruitFactor
 * \brief Factor for allocation to fruits
 *
 * Inputs
 * ------
 * - _minDensity_ is the crop density when the factor is 1 (no effect) [m<SUP>-2</SUP>]
 * - _maxValue_ is the maximum value of the factor [R<SUB>+</SUB>]
 * - _plantDensity_ is the crop density  [m<SUP>-2</SUP>]
 *
 * Output
 * ------
 * - _value_ is the value of the factor [>=1]
 *
 * Default dependencies
 * ------------
 * - a grandparent model with a _density_port [m<SUP>-2</SUP>]
 *
 */

FruitFactor::FruitFactor(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(minDensity, 1.);
    Input(maxValue, 1.);
    Input(plantDensity, "../..[density]");
    Output(value);
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

