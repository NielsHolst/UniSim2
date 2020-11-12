/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef OUTPUTS_H
#define OUTPUTS_H

#include <base/box.h>


namespace vg {

class Outputs : public base::Box
{
public:
    Outputs(QString name, QObject *parent);
    void amend();
private:
    // Inputs
    bool onlyNumbers;
    // Outputs
    double
        skyT,
        outdoorsT,
        outdoorsRh,
        outdoorsLight,
        outdoorsWindSpeed,
        soilT,
        coverT,
        screen1T,
        screen2T,
        screen3T,
        pipeT,
        leafT,
        floorT,
        indoorsT,
        indoorsRh,
        indoorsTotalPar,
        indoorsCo2,
        Uinside,
        Uoutside,
        spHeatingTemperature,
        spVentilationTemperature,
        spRhMax,
        spRhMin,
        spCrackVentilation,
        spScreenEnergyThreshold1,
        spScreenEnergyThreshold2,
        spScreenShadeThreshold1,
        spScreenShadeThreshold2,
        spScreenFixed1,
        spScreenFixed2,
        spScreenEnergyThresholdBand,
        spScreenShadeThresholdBand,
        spScreenCrack,
        spCo2,
        actHeatingPower,
        actVentilation,
        airTransmissivityScreens,
        airFluxTotal,
        airFluxPower,
        actPipeTempIn1,
        actPipeTempIn2,
        actPipeTempOut1,
        actPipeTempOut2,
        actLight1,
        actLight2,
        actLight3,
        actLightPower,
        actLightPar,
        actScreen1,
        actScreen2,
        actScreen3,
        actScreen1HeatCapacity,
        actCo2,
        vapourFluxTranspiration,
        vapourFluxCondensationCrop,
        vapourFluxCondensationCover,
        vapourFluxCondensationdScreens,
        vapourFluxVentilation,
        netPhotosynthesisRate,
        grossPhotosynthesisRate,
        darkRespirationRate,
        cropGrowthRate,
        lightUseEfficiency,
        totalIrradiation,
        totalSkyAbsorbed,
        totalSkySwAbsorbed,
        totalSkyLwAbsorbed,
        totalVentilationEnergy,
        totalConvectionEnergy,
        totalSoilEnergy,
        totalHeatingEnergy,
        totalGrowthLightEnergy,
        totalParAbsorbed,
        totalCo2;
    int thermostateUpdates;
    QString thermostatState, thermostatAction, thermostatSolution;
    void lookupPort(QString path);
};
} //namespace


#endif
