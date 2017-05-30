/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/exception.h>
#include "sliding_signal.h"

using namespace base;

namespace vg {
	
/*! \class SlidingSignal
* \brief A base class to control a signal according to the input
*
* The size of the _signal_ depends on where the _input_ is in relation to the interval [_threshold_; _threshold_+_thresholdBand_],
* and whether is is an _increasingSignal_.
* - An increasing signal goes from _minSignal_ for _input_ <  _threshold_, increasing to _maxSignal_ at _threshold_+_thresholdBand_ and above.
* - An decreasing signal goes from _maxSignal_ for _input_ <  _threshold_, decreasing to _minSignal_ at _threshold_+_thresholdBand_ and above.
*
* The exact course of the _signal_ in response to _input_ must be defined in a derived class, e.g. linear inside the thresholdBand for a
* ProportionalSignal.
*/

SlidingSignal::SlidingSignal(QString name, QObject *parent)
    : BaseSignal(name, parent)
{
    Class(SlidingSignal);
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
    QString test = fullName();
    if (input <= x0)
        res = y0;
    else if (input >= x1)
        res = y1;
    else {
        Q_ASSERT(thresholdBand != 0.);
        double gap = input - x0;
        res = y0 + slide(gap/thresholdBand)*(y1 - y0);
    }
    return res;
}

} //namespace

