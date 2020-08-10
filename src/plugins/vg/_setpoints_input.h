/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SETPOINTS_INPUT_H
#define SETPOINTS_INPUT_H

#include <base/box.h>

namespace vg {

class SetpointsInput : public base::Box
{
public:
    SetpointsInput(QString name, QObject *parent);
    void reset() final;
    void update() final;
private:
    // Inputs
    double heatingTemperatureAtLowRh, heatingTemperatureMargin,
        ventilationTemperatureMargin, ventilationTemperatureRhMargin,
        screenCrackAtHighRh, screenCrackAtHighTemperature,
        rhMax, rhMaxBand,
        co2Capacity, co2Setpoint, co2VentilationThreshold, co2VentilationBand,
        dawnThreshold, duskThreshold;
    // Outputs
    double ventilationTemperatureAtLowRh, ventilationTemperatureAtHighRh,
        heatingTemperatureAtHighRh,
        maxScreenAtHighRh, maxScreenAtHighTemperature;
};
} //namespace


#endif
