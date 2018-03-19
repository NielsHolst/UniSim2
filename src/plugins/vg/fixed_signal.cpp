/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "fixed_signal.h"

using namespace base;

namespace vg {

PUBLISH(FixedSignal)

FixedSignal::FixedSignal(QString name, QObject *parent)
    : BaseSignal(name, parent){
    help("simply sets the signal to the input");
    Input(input).equals(0).help("Value used for signal");
}

double FixedSignal::computeSignal() {
    return input;
}

} //namespace

