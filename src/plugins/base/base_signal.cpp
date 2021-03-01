/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "phys_math.h"
#include "test_num.h"
#include "base_signal.h"

using phys_math::infinity;

namespace base {

BaseSignal::BaseSignal(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(BaseSignal);
    help("is a base class classes that compute signal");
    Input(initialSignal).help("The value that signal is set to at reset");
    Input(initialFlag).equals(false).help("The value that flag is set to at reset");
    Input(maxChange).equals(infinity()).help("Maximum rate of change from previous step [per minute]");
    Input(timeStepSecs).importsMaybe("calendar[timeStepSecs]");
    Output(signal).help("Value of the signal");
    Output(flagIsUp).help("Is the signal different from zero?");
    Output(flagJustRaised).help("Did the signal change from false to true during the latest update?");
    Output(flagJustLowered).help("Did the signal change from true to false during the latest update?");
    NamedOutput("value", signal).help("Synonym for 'signal'");
}

void BaseSignal::reset() {
    signal = _prevSignal = initialSignal;
    flagIsUp = initialFlag;
    flagJustRaised = flagJustLowered = false;
    localReset();
}

void BaseSignal::update() {
    // Update flag
    bool oldFlag = flagIsUp;
    flagIsUp = computeFlag();
    flagJustRaised = !oldFlag && flagIsUp;
    flagJustLowered = oldFlag && !flagIsUp;

    // Update signal
    signal = computeSignal(flagIsUp);
    if (timeStepSecs > 0.)
        limitSignal();
    _prevSignal = signal;

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

