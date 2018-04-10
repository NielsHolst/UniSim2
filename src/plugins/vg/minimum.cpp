/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "minimum.h"

using namespace base;

namespace vg {

PUBLISH(Minimum)

Minimum::Minimum(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes minimum of inputs");
    Input(values).help("Input values");
    Output(value).help("Minimum of inputs; zero if no inputs");
}

void Minimum::reset() {
    value = 0;
    update();
}

void Minimum::update() {
    if (values.isEmpty()) return;
    value = values.at(0);
    for (double input : values)
        if (input < value) value = input;
}

} //namespace

