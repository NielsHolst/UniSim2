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
    Input(U).imports("construction/shelter[U]").unit("W/m2/K");
    Input(incomingLwAbsorptivity).imports("construction/shelter[incomingLwAbsorptivity]").unit("[0;1]");
    Input(outgoingLwAbsorptivity).imports("construction/shelter[outgoingLwAbsorptivity]").unit("[0;1]");
    Input(lightAbsorbedCover).imports("construction/shelter[lightAbsorbedCover]").unit("W/ground m2");
    Input(lightAbsorbedScreens).imports("construction/shelter[lightAbsorbedScreens]").unit("W/ground m2");
    Input(heatCapacityCover).imports("construction/shelter[heatCapacityCoversPerGround]").unit("J/ground m2");
    Input(heatCapacityScreens).imports("construction/shelter[heatCapacityScreensPerGround]").unit("J/ground m2");

    Input(timeStep).imports("calendar[timeStepSecs]").unit("s");
    Input(height).imports("geometry[indoorsAverageHeight]").unit("m");
    Input(coverPerGroundArea).imports("geometry[coverPerGroundArea]").unit("m2/m2");

    Input(indoorsTemperature).imports("indoors/temperature[value]").unit("oC");
    Input(outdoorsTemperature).imports("outdoors[temperature]").unit("oC");
    Input(skyTemperature).imports("outdoors[skyTemperature]").unit("oC");
    Input(radiationFluxCropTop).imports("crop/layers/top/radiationAbsorbed[shelterLoss]").unit("W/m2");
    Input(radiationFluxCropMiddle).imports("crop/layers/middle/radiationAbsorbed[shelterLoss]").unit("W/m2");
    Input(radiationFluxCropBottom).imports("crop/layers/bottom/radiationAbsorbed[shelterLoss]").unit("W/m2");

    Output(heatFluxOutsideToCover).help("Convergent flux from outside to cover").unit("W/m2");
    Output(heatFluxInsideToCover).help("Convergent flux from inside to cover").unit("W/m2");
    Output(radiationFluxOutsideToCover).help("Radiation flux from sky to cover (negative)").unit("W/m2");
    Output(radiationFluxInsideToCover).help("Radiation flux from inside to cover (negative)").unit("W/m2");
    Output(radiationFluxSunToCover).help("Radiation flux from sun to cover (positive)").unit("W/m2");
    Output(radiationFluxSunToScreens).help("Radiation flux from sun to screens (positive)").unit("W/m2");
    Output(coverTemperature).help("Temperature of cover").unit("oC");
    Output(screensTemperature).help("Temperature of screens").unit("oC");
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
        heatFluxInsideToCover  = U*(indoorsTemperature2 - coverTemperature)*coverPerGroundArea;;
        radiationFluxOutsideToCover = incomingLwAbsorptivity*Sigma*(p4K(skyTemperature) - p4K(coverTemperature))*coverPerGroundArea;
        radiationFluxInsideToCover  = incomingLwAbsorptivity*Sigma*(p4K(indoorsTemperature2) - p4K(coverTemperature))*coverPerGroundArea;

        radiationFluxSunToCover = lightAbsorbedCover;
        radiationFluxSunToScreens = lightAbsorbedScreens;

        double radiationFluxCropToShelter =
            radiationFluxCropTop + radiationFluxCropMiddle + radiationFluxCropBottom;
        double energyFluxToShelter =
                heatFluxOutsideToCover +
                heatFluxInsideToCover +
                radiationFluxOutsideToCover +
                radiationFluxInsideToCover +
                radiationFluxSunToCover +
                radiationFluxSunToScreens +
                radiationFluxCropToShelter;
        double energyFluxToInside =
                -heatFluxInsideToCover-radiationFluxInsideToCover;

        value += energyFluxToInside;
        coverTemperature += energyFluxToShelter*dt/(heatCapacityCover+heatCapacityScreens);
        indoorsTemperature2 += energyFluxToInside*dt/Cair;
    }
    value /= n;
}

} //namespace

