/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VENTILATION_BY_WIND_H
#define VG_VENTILATION_BY_WIND_H

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
    double baseRate, windSpeed, ventsProportionalEffectiveArea;
    // Output
    double value;
};
} //namespace


#endif
