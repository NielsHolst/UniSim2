/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_FLOOR_RADIATION_ABSORBED_H
#define VG_FLOOR_RADIATION_ABSORBED_H

#include <base/box.h>

namespace vg {

class FloorRadiationAbsorbed : public base::Box
{
public:
    FloorRadiationAbsorbed(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double indoorsLight, growthLightLight, lightAbsorbedByCrop,
        growthLightLw, growthLightLwAbsorbedByCrop;

    // Outputs
    double value;
};

} //namespace

#endif
