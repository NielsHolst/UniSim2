/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_ENERGY_FLUX_CONDENSATION_H
#define VG_ENERGY_FLUX_CONDENSATION_H

#include "energy_flux_base.h"

namespace vg {

class EnergyFluxCondensation : public EnergyFluxBase
{
public:
    EnergyFluxCondensation(QString name, QObject *parent);
    void update();

private:
    double vapourFlux;
};
} //namespace


#endif
