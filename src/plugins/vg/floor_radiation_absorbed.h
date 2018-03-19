/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
