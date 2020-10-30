/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "outputs.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(Outputs)

Outputs::Outputs(QString name, QObject *parent)
    : Box(name, parent)
{
    help("defines all model outputs");
    Output(skyT).imports("outdoors[skyTemperature]");
    Output(outdoorsT).imports("outdoors[temperature]");
    Output(outdoorsRh).imports("outdoors[rh]");
    Output(outdoorsLight).imports("outdoors[radiation]");
    Output(outdoorsWindSpeed).imports("outdoors[windSpeed]");
    Output(soilT).imports("outdoors[soilTemperature]");
    Output(coverT).imports("energyBudget/cover[temperature]");
    Output(screen1T).importsMaybe("energyBudget/screen1[temperature]");
    Output(screen2T).importsMaybe("energyBudget/screen2[temperature]");
    Output(screen3T).importsMaybe("energyBudget/screen3[temperature]");
    Output(pipeT).imports("energyBudget/pipe[temperature]");
    Output(leafT).imports("energyBudget/crop[temperature]");
    Output(floorT).imports("energyBudget/floor[temperature]");
    Output(indoorsT).imports("indoors/temperature[value]");
    Output(indoorsRh).imports("indoors/humidity[rh]");
    Output(indoorsTotalPar).imports("energyBudget[cropParFluxFromAbove]");
    Output(indoorsCo2).imports("indoors/co2[value]");
    Output(Uinside).imports("energyBudget[Uinside]");
    Output(Uoutside).imports("energyBudget[Uoutside]");
    Output(spHeatingTemperature).imports("setpoints/heatingTemperature[value]");
    Output(spVentilationTemperature).imports("setpoints/ventilationTemperature[value]");
    Output(spRhMax).imports("setpoints[rhMax]");
    Output(spRhMin).imports("setpoints[rhMin]");
    Output(spCrackVentilation).imports("setpoints/crackVentilation[value]");
    Output(spScreenEnergyThreshold1).imports("setpoints[screenEnergyThreshold1]");
    Output(spScreenEnergyThreshold2).imports("setpoints[screenEnergyThreshold2]");
    Output(spScreenShadeThreshold1).imports("setpoints[screenShadeThreshold1]");
    Output(spScreenShadeThreshold2).imports("setpoints[screenShadeThreshold2]");
    Output(spScreenFixed1).imports("setpoints[screenFixed1]");
    Output(spScreenFixed2).imports("setpoints[screenFixed2]");
    Output(spScreenEnergyThresholdBand).imports("setpoints[screenEnergyThresholdBand]");
    Output(spScreenShadeThresholdBand).imports("setpoints[screenShadeThresholdBand]");
    Output(spScreenCrack).imports("setpoints/screenCrack[value]");
    Output(spCo2).imports("setpoints[co2Setpoint]");
    Output(actHeatingPower).imports("actuators/heating[energyFluxTotal]");
    Output(actVentilation).imports("actuators/ventilation[value]");
    Output(airTransmissivityScreens).imports("indoors/ventilation[airTransmissivity]");
    Output(airFluxTotal).imports("indoors/ventilation[relative]");
    Output(airFluxPower).imports("indoors/temperature[advectiveEnergyFlux]");
    Output(actLight1).importsMaybe("actuators/growthLights/growthLight1[on]");
    Output(actLight2).importsMaybe("actuators/growthLights/growthLight2[on]");
    Output(actLight3).importsMaybe("actuators/growthLights/growthLight3[on]");
    Output(actLightPower).imports("actuators/growthLights[powerUsage]");
    Output(actLightPar).imports("actuators/growthLights[parFluxDown]");
    Output(actPipeTempIn1).importsMaybe("actuators/heating/pipes/pipe1[inflowTemperature]");
    Output(actPipeTempIn2).importsMaybe("actuators/heating/pipes/pipe2[inflowTemperature]");
    Output(actPipeTempOut1).importsMaybe("actuators/heating/pipes/pipe1[outflowTemperature]");
    Output(actPipeTempOut2).importsMaybe("actuators/heating/pipes/pipe2[outflowTemperature]");
    Output(actScreen1).importsMaybe("actuators/screens/layer1[value]");
    Output(actScreen2).importsMaybe("actuators/screens/layer2[value]");
    Output(actScreen3).importsMaybe("actuators/screens/layer3[value]");
    Output(actCo2).imports("actuators/co2Injection[value]");
    Output(vapourFluxTranspiration).imports("waterBudget/transpiration[vapourFlux]");
    Output(vapourFluxCondensationCrop).imports("waterBudget/condensationCrop[vapourFlux]");
    Output(vapourFluxCondensationCover).imports("waterBudget/condensationCover[vapourFlux]");
    Output(vapourFluxCondensationdScreens).imports("waterBudget/condensationScreens[vapourFlux]");
    Output(vapourFluxVentilation).imports("waterBudget/ventilation[vapourFlux]");
    Output(netPhotosynthesisRate).imports("crop/photosynthesis[Pn]");
    Output(grossPhotosynthesisRate).imports("crop/photosynthesis[Pg]");
    Output(darkRespirationRate).imports("crop/photosynthesis[Rd]");
    Output(cropGrowthRate).imports("crop/growth[netGrowthRate]");
    Output(lightUseEfficiency).imports("crop/photosynthesis[lue]");
    Output(totalIrradiation).imports("budget[skyIrradiation]");
    Output(totalSkyAbsorbed).imports("budget[skyRadiationAbsorbed]");
    Output(totalSkySwAbsorbed).imports("budget[skySwRadiationAbsorbed]");
    Output(totalSkyLwAbsorbed).imports("budget[skyLwRadiationAbsorbed]");
    Output(totalVentilationEnergy).imports("budget[ventilationEnergy]");
    Output(totalConvectionEnergy).imports("budget[convectionEnergy]");
    Output(totalSoilEnergy).imports("budget[soilEnergy]");
    Output(totalHeatingEnergy).imports("budget[heatingEnergy]");
    Output(totalGrowthLightEnergy).imports("budget[growthLightsEnergy]");
    Output(totalParAbsorbed).imports("budget[parAbsorbedTotal]");
    Output(totalCo2).imports("budget[co2Total]");
    // Output(thermostatAction).imports("energyBudgetOptimiser[action]");
    // Output(thermostatSolution).imports("energyBudgetOptimiser[solution]");
}

void Outputs::initialize() {
}


} //namespace
