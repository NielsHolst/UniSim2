/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VENTILATION_BY_TEMPERATURE_H
#define VENTILATION_BY_TEMPERATURE_H

#include <base/box.h>

namespace vg {

class VentilationByTemperature : public base::Box
{
public:
    VentilationByTemperature(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double coefficient, indoorsTemperature, outdoorsTemperature, ventsEffectiveArea, ventsMaxEffectiveArea, indoorsVolume;
    // Outputs
    double current, max;
    // Method
    void updateOutputs(double indoorsTemperature, double outdoorsTemperature);
};
} //namespace


#endif
