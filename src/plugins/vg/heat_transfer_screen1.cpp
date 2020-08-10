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
    help("holds the combined radiative parameters of screen layer 1");
    /* density of polyester = 1.22 g/cm3
     * 1mm * 1m2 = 0.1cm * 100^2 cm2 = 1000 cm3 = 1220 g/m2
     * heat capacity of polyester = 1.87 J/g/K = 1.87*1220 J/m2/K = 2280 J/m2/K
    */
    port("specificHeatCapacity")->equals(2280.);
    port("condensationRate")->imports("waterBudget/condensationScreens/screen1[vapourFlux]");
}

void HeatTransferScreen1::update() {
    updateArea();
    updateRadiativeProperties();
    updateLwEmission();
    updateConvectiveProperties();
    updateTemperature();
}

} //namespace

