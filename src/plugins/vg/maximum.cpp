/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "maximum.h"

using namespace base;

namespace vg {

PUBLISH(Maximum)

Maximum::Maximum(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes maximum of inputs");
    Input(values).help("Input values");
    Output(value).help("Maximum of inputs; zero if no inputs");
}

void Maximum::reset() {
    value = 0;
    update();
}

void Maximum::update() {
    if (values.isEmpty()) return;
    value = values.at(0);
    for (double input : values)
        if (input > value) value = input;
}

} //namespace

