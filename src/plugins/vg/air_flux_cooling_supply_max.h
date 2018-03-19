/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AIR_FLUX_COOLING_SUPPLY_MAX_H
#define AIR_FLUX_COOLING_SUPPLY_MAX_H

#include <base/box.h>

namespace vg {

class AirFluxCoolingSupplyMax : public base::Box
{
public:
    AirFluxCoolingSupplyMax(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double byWind, byTemperature, givenAirflux;
    // Output
    double value;
};
} //namespace


#endif
