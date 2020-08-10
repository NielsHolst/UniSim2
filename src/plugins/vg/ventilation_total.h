/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VENTILATION_TOTAL_H
#define VENTILATION_TOTAL_H

#include <base/box.h>

namespace vg {

class VentilationTotal : public base::Box
{
public:
    VentilationTotal(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double effectiveArea, groundArea,
        leakage, windSpeed, outdoorsTemperature, indoorsTemperature,
        windCoef, temperatureCoef,
        state;
    // Outputs
    double value, dueToLeakage, dueToVents,
        relativeArea, fluxLeak, fluxWind, fluxTemp;
};

} //namespace


#endif
