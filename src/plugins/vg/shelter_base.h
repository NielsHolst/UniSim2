/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_SHELTER_BASE_H
#define VG_SHELTER_BASE_H

#include "surface_radiation_outputs.h"

namespace vg {

class ShelterBase : public SurfaceRadiationOutputs
{
public:
    ShelterBase(QString name, QObject *parent);
    void reset();

protected:
    // Outputs
    double diffuseLightTransmitted, directLightTransmitted, totalLightTransmitted,
        lightAbsorbedCover, lightAbsorbedScreens,
        haze, U,
        airTransmissivity;
};
} //namespace


#endif
