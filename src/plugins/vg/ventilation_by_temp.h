/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VENTILATION_MAX_BY_TEMP_H
#define VENTILATION_MAX_BY_TEMP_H

#include <base/box.h>

namespace vg {

class VentilationMaxByTemp : public base::Box
{
public:
    VentilationMaxByTemp(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double indoorsTemperature, outdoorsTemperature, windowLength, windowHeight, greenhouseArea,
        dischargeCoefficient;
    // Output
    double value;
};
} //namespace


#endif
