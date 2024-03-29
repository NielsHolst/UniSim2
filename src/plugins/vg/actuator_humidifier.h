/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ACTUATOR_HUMIDIFIER_H
#define ACTUATOR_HUMIDIFIER_H
#include <base/box.h>


namespace vg {

class ActuatorHumidifier : public base::Box
{
public: 
    ActuatorHumidifier(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double groundArea, greenhouseVolume, timeStep, inflowTemperature, inflowRh, efficiency, maxHumidification,
        state;
    // Outputs
    double coolingPower, vapourFlux;
};

} //namespace
#endif
