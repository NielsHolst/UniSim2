/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <limits>
#include <base/publish.h>
#include "accumulator.h"
#include "utilities.h"

using namespace base;

namespace vg {

PUBLISH(Accumulator)

Accumulator::Accumulator(QString name, QObject *parent)
    : Box(name, parent)
{
    help("accumulates values");
    Input(initial).help("Initial value at reset");
    Input(change).help("Change added to value");
    Input(minValue).equals(std::numeric_limits<double>::lowest()).help("Minimum possible value");
    Input(maxValue).equals(std::numeric_limits<double>::max()).help("Maximum possible value");
    Output(value).help("Accumulated value");
}

void Accumulator::reset() {
    value = initial;
    change = 0.;
    update();
}

void Accumulator::update() {
    value += change;
    value = minMax(minValue, value, maxValue);
}

} //namespace

