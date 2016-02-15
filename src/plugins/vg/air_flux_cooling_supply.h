/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef AIR_FLUX_COOLING_SUPPLY_H
#define AIR_FLUX_COOLING_SUPPLY_H

#include <base/box.h>

namespace vg {

class AirFluxCoolingSupply : public base::Box
{
public:
    AirFluxCoolingSupply(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs;
    double energyFlux, airSupplyMax, indoorsTemperature, outdoorsTemperature, indoorsHeight;
    // Outputs
    double value;
};
} //namespace


#endif
