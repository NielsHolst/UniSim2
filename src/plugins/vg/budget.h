/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BUDGET_H
#define BUDGET_H

#include <base/box.h>

namespace vg {

class Budget : public base::Box
{
public:
    Budget(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Inputs
    QDate date;
    double
        dt,
        skyIrradiationRate,
        skyRadiationAbsorbedRate, skySwRadiationAbsorbedRate, skyLwRadiationAbsorbedRate,
        ventilationEnergyRate,
        convectionEnergyRate,
        groundArea,
        indoorsAh, outdoorsAh,
        soilEnergyRate,
        heatingPowerUsage,
        growthLightsPowerUsage,
         parAbsorbed,
        co2Flux;
    // Outputs
    double
        skyIrradiation,
        skyRadiationAbsorbed, skySwRadiationAbsorbed, skyLwRadiationAbsorbed,
        ventilationEnergy,
        convectionEnergy,
        latentHeatEnergy,
        soilEnergy,
        heatingEnergy,
        growthLightsEnergy,
        parAbsorbedTotal,
        co2Total;
    // Data
    double _kiloHour, _kiloMol;
    QDate _prevDate;
    bool _hasResetSums;
    // Method
    void checkNewYear();

};
} //namespace


#endif
