/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VG_ENERGY_FLUX_SHELTER_H
#define VG_ENERGY_FLUX_SHELTER_H

#include "energy_flux_base.h"

namespace vg {

class EnergyFluxShelter : public EnergyFluxBase
{
public:
    EnergyFluxShelter(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double U, incomingLwAbsorptivity,
        outgoingLwAbsorptivity,
        lightAbsorbedCover, lightAbsorbedScreens,
        heatCapacityCover, heatCapacityScreens,
        timeStep, height,
        coverPerGroundArea,
        indoorsTemperature, skyTemperature, outdoorsTemperature,
        radiationFluxCropTop, radiationFluxCropMiddle, radiationFluxCropBottom;

    // Outputs
    double coverTemperature, screensTemperature,
        heatFluxOutsideToCover, heatFluxInsideToCover,
        radiationFluxSkyToCover,
        radiationFluxSunToCover, radiationFluxSunToScreens;

};
} //namespace


#endif
