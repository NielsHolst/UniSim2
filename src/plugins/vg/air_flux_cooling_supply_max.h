/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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
    double byWind, byTemp, givenAirflux;
    // Output
    double value;
};
} //namespace


#endif
