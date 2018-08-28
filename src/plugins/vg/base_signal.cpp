/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/test_num.h>
#include "base_signal.h"

using namespace base;
using phys_math::infinity;

namespace vg {

BaseSignal::BaseSignal(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(BaseSignal);
    help("is a base class classes that compute signal");
    Input(signalReset).help("The value that signal is set to at reset");
    Input(maxChange).equals(infinity()).help("Maximum rate of change from previous step [per minute]");
    Input(timeStepSecs).importsMaybe("calendar[timeStepSecs]");
    Output(signal).help("Value of the signal");
    Output(flag).help("Is the signal different from zero?");
    Output(flagUp).help("Did the signal change from false to true during the latest update?");
    Output(flagDown).help("Did the signal change from true to false during the latest update?");
    NamedOutput("value", signal).help("Synonym for 'signal'");
}

void BaseSignal::reset() {
    signal = _prevSignal = signalReset;
    flag = (signal!=0.);
    flagUp = flagDown = false;
    localReset();
}

void BaseSignal::update() {
    signal = computeSignal();
    if (timeStepSecs > 0.)
        limitSignal();
    _prevSignal = signal;

    bool oldFlag = flag;
    flag = TestNum::neZero(signal);
    flagUp = !oldFlag && flag;
    flagDown = oldFlag && !flag;
}

void BaseSignal::limitSignal() {
    double dt = timeStepSecs/60.,
           change = (signal - _prevSignal)/dt;
    if (fabs(change) > maxChange) {
        double sign = (change < 0.) ? -1. : 1.;
        signal = _prevSignal + sign*maxChange*dt;
    }
}

} //namespace

