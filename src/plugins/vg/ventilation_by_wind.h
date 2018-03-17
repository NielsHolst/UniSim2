/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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
