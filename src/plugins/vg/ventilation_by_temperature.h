/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
