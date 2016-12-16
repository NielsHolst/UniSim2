/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <limits>
#include "random_base.h"

using namespace base;

namespace boxes {

RandomBase::RandomBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(minValue).equals(-std::numeric_limits<double>::infinity());
    Input(maxValue).equals(std::numeric_limits<double>::infinity());
    Input(maxTries).equals(100);
    Input(drawAtReset).equals(true);
    Input(drawAtUpdate).equals(false);
    Output(value);
}

void RandomBase::initialize() {
    doInitialize();
    nextValue();
}

void RandomBase::reset() {
    if (drawAtReset) nextValue();
}

void RandomBase::update() {
    if (drawAtUpdate) nextValue();
}

void RandomBase::nextValue() {
    int i = 0;
    do {
        value = drawValue();
        if (++i == maxTries) {
            QString msg = "Max number of tries exceeded to find random value inside interval: %1 to %2";
            ThrowException(msg.arg(minValue).arg(maxValue)).value(maxTries).context(this);
        }
    } while (value < minValue || value > maxValue);
}

} //namespace
