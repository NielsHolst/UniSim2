/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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
        radiationFluxOutsideToCover, radiationFluxInsideToCover,
        radiationFluxSunToCover, radiationFluxSunToScreens;

};
} //namespace


#endif
