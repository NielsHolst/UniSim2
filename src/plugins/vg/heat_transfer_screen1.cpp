/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "heat_transfer_screen1.h"

using namespace base;

namespace vg {

PUBLISH(HeatTransferScreen1)

HeatTransferScreen1::HeatTransferScreen1(QString name, QObject *parent)
    : HeatTransferShelter(name, parent, "shelter/*/screens/layer1", "Net")
{
    help("holds the radiative parameters of screen layer 1 averaged for all shelter faces");
    Input(keepTemperatureConstant).imports("energyBudget[keepConstantScreenTemperature]", CA);
    port("condensationRate")->imports("waterBudget/condensationScreens/screen1[vapourFlux]");
    port("maxTemperatureRateOfChange")->equals(0.25);
}

void HeatTransferScreen1::update() {
    updateArea();
    updateHeatCapacity();
    updateRadiativeProperties();
    updateLwEmission();
    updateConvectiveProperties();
    if (keepTemperatureConstant || ++tick_ < 10)
        temperature = temperatureTop = temperatureBottom = indoorsTemperature;
    else
        updateTemperature();
}

} //namespace

