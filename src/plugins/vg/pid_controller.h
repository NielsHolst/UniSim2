/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
