/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_BASE_GROWTH_LIGHT_H
#define VG_BASE_GROWTH_LIGHT_H

#include <base/box.h>

namespace vg {

class GrowthLightBase : public base::Box
{
public:
    GrowthLightBase(QString name, QObject *parent);
protected:
    // Outputs
    double heatEmission, longWaveEmission, shortWaveEmission, totalEmission, parEmission, photonIntensity, energyFlux;
    bool currentlyOn;
};
} //namespace


#endif
