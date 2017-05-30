/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/publish.h>
#include "accumulator.h"

using namespace base;

namespace vg {

PUBLISH(Accumulator)

Accumulator::Accumulator(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initial).help("Initial value at reset");
    Input(change).help("Change added to value");
    Output(value).help("Accumulated value");
}

void Accumulator::reset() {
    value = initial;
}

void Accumulator::update() {
    value += change;
}

} //namespace

