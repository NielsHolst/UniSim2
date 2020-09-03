/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ACTUATOR_VENTILATION_H
#define ACTUATOR_VENTILATION_H

#include <base/box.h>

namespace vg {

class ActuatorVentilation : public base::Box
{
public:
    ActuatorVentilation(QString name, QObject *parent);
    void reset();
    double getOpening() const;
    void setOpening(double opening);
private:
    // Inputs
    double value, minValue, maxValue;
};
} //namespace


#endif
