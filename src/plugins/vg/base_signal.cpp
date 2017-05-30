/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/test_num.h>
#include "base_signal.h"

using namespace base;

namespace vg {

BaseSignal::BaseSignal(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(BaseSignal);
    Input(signalReset).help("The value that signal is set to at reset");
    Output(signal).help("Value of the signal");
    Output(flag).help("Is the signal is different from zero?");
    Output(flagUp).help("Did the signal change from false to true during the latest update?");
    Output(flagDown).help("Did the signal change from true to false during the latest update?");
}

void BaseSignal::reset() {
    signal = signalReset;
    flag = (signal!=0.);
    flagUp = flagDown = false;
    localReset();
}

void BaseSignal::update() {
    signal = computeSignal();
    bool oldFlag = flag;
    flag = TestNum::neZero(signal);
    flagUp = !oldFlag && flag;
    flagDown = oldFlag && !flag;
}

} //namespace

