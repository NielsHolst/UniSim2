/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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
    double heatEmission, longWaveEmission, shortWaveEmission, totalEmission, parEmission, energyFlux;
    bool currentlyOn;
};
} //namespace


#endif
