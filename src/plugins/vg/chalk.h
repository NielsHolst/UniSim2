/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CHALK_H
#define CHALK_H
#include <base/box.h>


namespace vg {

class Chalk : public base::Box
{
public: 
    Chalk(QString name, QObject *parent=0);
    void reset();
    void update();
private:
    // Inputs
    double setpoint, maxReflectance;
    // Outputs
    double value;
};

} //namespace
#endif
