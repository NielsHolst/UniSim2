/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_CONTROL_ELEMENT_BASE_H
#define BASE_CONTROL_ELEMENT_BASE_H

#include <base/box.h>

namespace vg {

class BaseControlElement : public base::Box
{
public:
    BaseControlElement(QString name, QObject *parent);
    void reset() final;
    void update() final;
    virtual void localReset() {}

    // Inputs
protected:
    double initial, target, timeStep;
private:

    double minimum, maximum, minSlope, maxSlope;

    // Outputs
    double state, value, slope;

    // Data
    double state0, state1, state2;
    int tick;

    // Methods
    virtual double change(double error) = 0;
};
} //namespace


#endif
