/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H

#include <base/box.h>

namespace vg {

class Accumulator : public base::Box
{
public:
    Accumulator(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double initial, change, minValue, maxValue;
    // Outputs
    double value;
};
} //namespace


#endif
