/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SKY_TEMPERATURE_H
#define SKY_TEMPERATURE_H

#include <base/box.h>

namespace vg {

class SkyTemperature : public base::Box
{
public:
    SkyTemperature(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double slope, intercept, airTemperature, rh;
    // Output
    double temperature, emissivity;
    // Data
    int tick;
};
} //namespace


#endif
