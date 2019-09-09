/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <limits>
#include <base/test_num.h>
#include "base_control_element.h"
#include "general.h"
#include "utilities.h"

using namespace base;

namespace vg {

/*! \class BaseControlElement
 * \brief Base class for control elements which maintain a _state_ in response to a _signal_
 *
 * Depending on the logic implemented in the _change_ method, _state_ will approach
 * _signal_ with time
 *
 * Inputs
 * ------
 * - _initState_ is the initial state when the control element is reset [-]
 * - _minimum_ is the minimum value that state can attain [-]
 * - _maximum_ is the maximum value that state can attain [-]
 * - _minSlope_ is the minimum slope that the rate of change in state can attain [min<SUP>-1</<SUP>]
 * - _maxSlope_ is the maximum slope that the rate of change in state can attain [min<SUP>-1</<SUP>]
 * - _timeStep_ is the integration time step [s]
 * - _signal_ is the desired state [-]
 *
 * Outputs
 * ------
 * - _state_ is the current state [-]
 * - _value_ is synonumous with _state_ [-]
 * - _slope_ is the rate of change in state [min<SUP>-1</<SUP>]
 */

BaseControlElement::BaseControlElement(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(BaseControlElement);
    Input(initial).help("The initial value at reset");
    Input(minimum).equals(-std::numeric_limits<double>::max()).help("Minimum possible value");
    Input(maximum).equals(std::numeric_limits<double>::max()).help("Maximum possible value");
    Input(minSlope).equals(-std::numeric_limits<double>::max()).help("Minimum possible rate of change in value (per min)");
    Input(maxSlope).equals(std::numeric_limits<double>::max()).help("Maximum possible rate of change in value (per min)");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(target).help("Target value");
    Output(value);
    Output(slope);
}

void BaseControlElement::reset() {
    state = value = state0 = state1 = state2 = initial;
    slope = 0;
    tick = 0;
    localReset();
}

void BaseControlElement::update() {
    state0 = state1;
    state1 = state2;
    state2 = state;
    state += change(target - state);
    double dt = timeStep/60.;
    slope = fitSlopePPP(state1, state2, state)/dt;
    if (tick++>10) {
        if (slope < minSlope) {
            state = fitPointPPS(state1, state2, minSlope*dt);
            slope = minSlope;
        }
        else if (slope > maxSlope) {
            state = fitPointPPS(state1, state2, maxSlope*dt);
            slope = maxSlope;
        }
    }
    state = value = minMax(minimum, state, maximum);
}

} //namespace
