/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_ENERGY_FLUX_COOLING_SUPPLY_H
#define VG_ENERGY_FLUX_COOLING_SUPPLY_H

#include "energy_flux_base.h"

namespace vg {

class EnergyFluxCoolingSupply : public EnergyFluxBase
{
public:
    EnergyFluxCoolingSupply(QString name, QObject *parent);
    void update();

private:
    // Inputs
    double airSupplyMax, energyDemand,
        indoorsTemperature, outdoorsTemperature, height, airTransmissivity;
};
} //namespace


#endif
