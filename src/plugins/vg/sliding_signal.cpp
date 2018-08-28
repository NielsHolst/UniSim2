/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include "sliding_signal.h"

using namespace base;

namespace vg {

SlidingSignal::SlidingSignal(QString name, QObject *parent)
    : BaseSignal(name, parent)
{
    Class(SlidingSignal);
    help("base class for computing a signal that slides with an input");
    Input(input).equals(25.).help("Value determining the signal");
    Input(threshold).equals(25.).help("Input threshold at which the signal begins changing");
    Input(thresholdBand).equals(5.).help("Interval of the input over which the signal is changing");
    Input(minSignal).equals(0.).help("Minimum possible signal");
    Input(maxSignal).equals(100.).help("Maximum possible signal");
    Input(increasingSignal).equals(true).help("Is the signal increasing inside the threshold band?");
}

double SlidingSignal::computeSignal() {
    double res;
    double x0{threshold},
           x1{threshold + thresholdBand},
           y0{increasingSignal ? minSignal : maxSignal},
           y1{increasingSignal ? maxSignal : minSignal};
    if (thresholdBand < 0)
        std::swap(x0, x1);
    if (input <= x0)
        res = y0;
    else if (input >= x1)
        res = y1;
    else {
        Q_ASSERT(thresholdBand != 0.);
        double gap = input - x0;
        res = y0 + slide(gap/fabs(thresholdBand))*(y1 - y0);
    }
    return res;
}

} //namespace

