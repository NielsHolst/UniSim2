/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ACTUATOR_PAD_AND_FAN_H
#define ACTUATOR_PAD_AND_FAN_H
#include <base/box.h>


namespace vg {

class ActuatorPadAndFan : public base::Box
{
public: 
    ActuatorPadAndFan(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double groundArea, outdoorsTemperature, outdoorsRh, efficiency, maxFlowRate,
        state;
    // Outputs
    double coolingPower, vapourFlux;
};

} //namespace
#endif