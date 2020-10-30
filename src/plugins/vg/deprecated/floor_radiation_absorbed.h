/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FLOOR_RADIATION_ABSORBED_H
#define FLOOR_RADIATION_ABSORBED_H

#include <base/box.h>

namespace vg {

class FloorRadiationAbsorbed : public base::Box
{
public:
    FloorRadiationAbsorbed(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double indoorsLight, growthLightLight, reflectance,
        lightAbsorbedByCrop,
        growthLightLw, growthLightLwAbsorbedByCrop;

    // Outputs
    double value, reflected;
};

} //namespace

#endif
