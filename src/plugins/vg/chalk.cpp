/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "chalk.h"

using namespace base;

namespace vg {

PUBLISH(Chalk)

Chalk::Chalk(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes average of inputs");
    Input(setpoint).help("Setpoint for chalk").unit("[0;1]");
    Input(maxReflectance).equals(0.8).help("Maximum reflectance at full chalk coverage").unit("[0;1]");
    Output(value).help("Reflectance caused by chalk").unit("[0;1]");
}

void Chalk::reset() {
    update();
}

void Chalk::update() {
    value = maxReflectance*setpoint;
}

} //namespace

