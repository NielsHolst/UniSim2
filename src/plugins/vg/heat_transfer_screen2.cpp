/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "heat_transfer_screen2.h"

using namespace base;

namespace vg {

PUBLISH(HeatTransferScreen2)

HeatTransferScreen2::HeatTransferScreen2(QString name, QObject *parent)
    : HeatTransferShelter(name, parent, "shelter/*/screens/layer2", "Net")
{
    help("holds the combined radiative parameters of screen layer 2");
    port("specificHeatCapacity")->equals(2280.);
    port("condensationRate")->imports("waterBudget/condensationScreens/screen2[vapourFlux]");
}

void HeatTransferScreen2::update() {
    updateArea();
    updateRadiativeProperties();
    updateLwEmission();
    updateConvectiveProperties();
    updateTemperature();
}

} //namespace

