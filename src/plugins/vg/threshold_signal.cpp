/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "threshold_signal.h"

using namespace base;

namespace vg {

PUBLISH(ThresholdSignal)

ThresholdSignal::ThresholdSignal(QString name, QObject *parent)
    : BaseSignal(name, parent){
    help("sets a signal according to a threshold value");
    Input(threshold).help("Threshold value");
    Input(input).help("Input that is compared to threshold");
    Input(signalBelow).help("Signal when value is below threshold");
    Input(signalAbove).equals(1.).help("Signal when value is at or above threshold");
}

double ThresholdSignal::computeSignal() {
    return (input < threshold) ? signalBelow : signalAbove;
}

} //namespace

