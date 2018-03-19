/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VENTILATION_BY_WIND_H
#define VENTILATION_BY_WIND_H

#include <base/box.h>

namespace vg {

class VentilationByWind : public base::Box
{
public:
    VentilationByWind(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double coefficient, windSpeed, ventsEffectiveArea, ventsMaxEffectiveArea, indoorsVolume;
    // Output
    double current, max;
    // Data
    double _maxValue, _slope;
    // Method
    void updateOutputs(double windSpeed);
};
} //namespace


#endif
