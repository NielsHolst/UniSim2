/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SOIL_TEMPERATURE_H
#define SOIL_TEMPERATURE_H

#include <base/box.h>

namespace PestTox {

class SoilTemperature : public base::Box
{
public:
    SoilTemperature(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Tmin, Tmax, dayLength;

    // Output
    double value;
};

} //namespace


#endif
