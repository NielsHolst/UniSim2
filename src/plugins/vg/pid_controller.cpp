/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "pid_controller.h"
#include <base/publish.h>

using namespace base;

namespace vg {
	
PUBLISH(PidController)

PidController::PidController(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(sensedValue).help("The sensed value");
    Input(desiredValue).help("The desired value (setpoint)");
    Input(controlVariableReset).help("The initial value of the control variable at reset");
    Input(Kprop).equals(0.5).help("The proportional gain (per min)");
    Input(Kint).equals(0.).help("The integral gain (per min)");
    Input(Kderiv).equals(0.).help("The derivative gain (per min)");

    Input(minimum).equals(-std::numeric_limits<double>::max()).help("Minimum allowed value of control variable");
    Input(maximum).equals(std::numeric_limits<double>::max()).help("Maximum allowed value of control variable");
    Input(minSlope).equals(-std::numeric_limits<double>::max()).help("Minimum allowed rate if change in control variable (per min)");
    Input(maxSlope).equals(std::numeric_limits<double>::max()).help("Maximum allowed rate if change in control variable (per min)");
    Input(timeStep).imports("calendar[timeStepSecs]");

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
    qBound(controlVariable, minimum, maximum);
    controlVariableSlope = slope;
}

} //namespace

