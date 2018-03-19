/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include "energy_flux_shelter.h"
#include "general.h"
#include <base/publish.h>

using std::max;
using namespace base;

namespace vg {
	
PUBLISH(EnergyFluxShelter)

EnergyFluxShelter::EnergyFluxShelter(QString name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    help("models shelter temperature and energy flux");
    Input(U).imports("construction/shelter[U]");
    Input(incomingLwAbsorptivity).imports("construction/shelter[incomingLwAbsorptivity]");
    Input(outgoingLwAbsorptivity).imports("construction/shelter[outgoingLwAbsorptivity]");
    Input(lightAbsorbedCover).imports("construction/shelter[lightAbsorbedCover]");
    Input(lightAbsorbedScreens).imports("construction/shelter[lightAbsorbedScreens]");
    Input(heatCapacityCover).imports("construction/shelter[heatCapacityCoversPerGround]");
    Input(heatCapacityScreens).imports("construction/shelter[heatCapacityScreensPerGround]");

    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(height).imports("geometry[indoorsAverageHeight]");
    Input(coverPerGroundArea).imports("geometry[coverPerGroundArea]");

    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(skyTemperature).imports("outdoors[skyTemperature]");
    Input(radiationFluxCropTop).imports("crop/layers/top/radiationAbsorbed[shelterLoss]");
    Input(radiationFluxCropMiddle).imports("crop/layers/middle/radiationAbsorbed[shelterLoss]");
    Input(radiationFluxCropBottom).imports("crop/layers/bottom/radiationAbsorbed[shelterLoss]");

    Output(heatFluxOutsideToCover).help("Energy flux from inside to cover [W/m2]");
    Output(heatFluxInsideToCover).help("Energy flux from inside to cover [W/m2]");
    Output(radiationFluxSkyToCover).help("Energy flux from sky to cover (negative) [W/m2]");
    Output(radiationFluxSunToCover).help("Energy flux from sun to cover (positive) [W/m2]");
    Output(radiationFluxSunToScreens).help("Energy flux from sun to screens (positive) [W/m2]");
    Output(coverTemperature).help("Temperature of cover [oC]");
    Output(screensTemperature).help("Temperature of screens [oC]");
}

void EnergyFluxShelter::reset() {
    coverTemperature = screensTemperature = 12.;
}

void EnergyFluxShelter::update() {
    const int maxTimeStep = 20;     // Use time steps no larger than this [s]
    double Cair = height*RhoAir*CpAir;
    int n = max(int(timeStep/maxTimeStep), 1);
    double dt = timeStep/n;
    value = 0.;
    double indoorsTemperature2 = indoorsTemperature;
    // All fluxes are signed relative to the shelter:
    //  a positive flux means shelter is gaining energy
    //  a negative flux means shelter is loosing energy
    for (int i=0; i < n; ++i) {
        screensTemperature = (coverTemperature + indoorsTemperature2)/2;
        // W/m2 ground = W/m2 cover * m2 cover / m2 ground
        heatFluxOutsideToCover = U*(outdoorsTemperature - coverTemperature)*coverPerGroundArea;;
        heatFluxInsideToCover = U*(indoorsTemperature2 - coverTemperature)*coverPerGroundArea;;
        radiationFluxSkyToCover = incomingLwAbsorptivity*Sigma*(p4K(skyTemperature) - p4K(coverTemperature))*coverPerGroundArea;

        radiationFluxSunToCover = lightAbsorbedCover;
        radiationFluxSunToScreens = lightAbsorbedScreens;

        double radiationFluxCropToShelter =
            radiationFluxCropTop + radiationFluxCropMiddle + radiationFluxCropBottom;
        double radiationFluxToShelter =
                heatFluxOutsideToCover +
                heatFluxInsideToCover +
                radiationFluxSkyToCover +
                radiationFluxSunToCover +
                radiationFluxSunToScreens +
                radiationFluxCropToShelter;
        double radiationFluxToInside =
                -heatFluxInsideToCover;

        value += radiationFluxToInside;
        coverTemperature += radiationFluxToShelter*dt/(heatCapacityCover+heatCapacityScreens);
        indoorsTemperature2 += radiationFluxToInside*dt/Cair;
    }
    value /= n;
}

} //namespace

