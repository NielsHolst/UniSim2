/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <base/box.h>

namespace vg {

class PidController : public base::Box
{
public:
    PidController(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double sensedValue, desiredValue, controlVariableReset,
        minimum, maximum, minSlope, maxSlope, timeStep,
        Kprop, Kint, Kderiv;
    // Outputs
    double controlVariable, controlVariableSlope,
        error, errorIntegral, errorDerivative;
    // Data
    double dt, prevError, cv0, cv1;
    int tick;
    // Methods
    void updateControlVariable();
    void adjustControlVariable();
};
} //namespace


#endif
