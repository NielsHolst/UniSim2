/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CO2_CONTROLLER_H
#define CO2_CONTROLLER_H
#include <base/box.h>

namespace vg {

class Co2Controller : public base::Box
{
public:
    Co2Controller(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double minCo2, maxCo2, indoorsCo2, injectionRate, timeStep;
    // Outputs
    double signal;
    // Data
    bool on;
};
} //namespace


#endif
