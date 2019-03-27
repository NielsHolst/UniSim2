/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "pid_controller.h"
#include <base/phys_math.h>
#include <base/publish.h>

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(PidController)

const double doubleMax = std::numeric_limits<double>::max();

PidController::PidController(QString name, QObject *parent)
    : Box(name, parent)
{
    help("delivers PID control of a control variable");
    Input(sensedValue).help("The sensed value");
    Input(desiredValue).help("The desired value (setpoint)");
    Input(controlVariableReset).help("The initial value of the control variable at reset");
    Input(Kprop).equals(0.5).help("The proportional gain").unit("/min");
    Input(Kint).equals(0.).help("The integral gain").unit("/min");
    Input(Kderiv).equals(0.).help("The derivative gain").unit("/min");

    Input(minimum).equals(-doubleMax).help("Minimum allowed value of control variable");
    Input(maximum).equals(doubleMax).help("Maximum allowed value of control variable");
    Input(minSlope).equals(-doubleMax).help("Minimum allowed rate of change in control variable").unit("/min");
    Input(maxSlope).equals(doubleMax).help("Maximum allowed rate of change in control variable").unit("/min");
    Input(timeStep).imports("calendar[timeStepSecs]").unit("s");

    Output(controlVariable).help("The control variable; tends to zero when all three error terms summed tend to zero");
    Output(controlVariableSlope).help("Slope of change in control variable (per min)");
    Output(error).help("The error");
    Output(errorIntegral).help("The integral error");
    Output(errorDerivative).help("The derivative error");
}

void PidController::reset() {
    controlVariable = cv0 = cv1 = controlVariableReset;
    dt = timeStep/60.;  // from secs to minutes
    tick = 0;
}

void PidController::update() {
    updateControlVariable();
    adjustControlVariable();
    prevError = error;
    cv0 = cv1;
    cv1 = controlVariable;
    ++tick;
}

void PidController::updateControlVariable() {
    error = desiredValue - sensedValue;
    errorIntegral += error;
    errorDerivative = (tick == 0) ? 0. : (error - prevError);
    controlVariable = (Kprop*error + Kint*errorIntegral + Kderiv*errorDerivative)*dt;
}

void PidController::adjustControlVariable() {
    double slope = fitSlopePPP(cv0, cv1, controlVariable)/dt;
    if (tick>10) {
        if (slope < minSlope) {
            controlVariable = fitPointPPS(cv0, cv1, minSlope*dt);
            slope = minSlope;
        }
        else if (slope > maxSlope) {
            controlVariable = fitPointPPS(cv0, cv1, maxSlope*dt);
            slope = maxSlope;
        }
    }
    controlVariable = qBound(minimum, controlVariable, maximum);
    controlVariableSlope = slope;
}

} //namespace

