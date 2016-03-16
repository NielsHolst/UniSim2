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

/*! \class BaseSignal
 * \brief Base class for models that set a signal
 *
 * The _signal_ function in the derived class defines the behaviour.
 *
 * Inputs
 * ------
 * - _signalReset_ is the value that _signal_ is set to at reset [R]
 *
 * Output
 * ------
 * - _signal_ is the value of the signal [R]
 * - _flag_ is true is _signal_ is different from zero [true,false]
 * - _flagUp_ is true if _flag_ just went from false to true [true,false]
 * - _flagDown_ is true if _flag_ just went from true to false [true,false]
 */

BaseSignal::BaseSignal(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(BaseSignal);
    Input(signalReset);
    Output(signal);
    Output(flag);
    Output(flagUp);
    Output(flagDown);
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

