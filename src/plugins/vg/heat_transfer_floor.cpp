/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "heat_transfer_floor.h"

using namespace base;

namespace vg {

PUBLISH(HeatTransferFloor)

HeatTransferFloor::HeatTransferFloor(QString name, QObject *parent)
    : HeatTransferLayerBase(name, parent)
{
    help("holds the radiative parameters of the floor");
    port("swTransmissivityTop")->equals(0.);
    port("swTransmissivityBottom")->equals(0.);
    port("swReflectivityTop")->equals(0.4);
    port("swReflectivityBottom")->equals(0.4);
    port("lwTransmissivityTop")->equals(0.);
    port("lwTransmissivityBottom")->equals(0.);
    port("lwReflectivityTop")->equals(0.4);
    port("lwReflectivityBottom")->equals(0.4);
    port("Utop")->equals(7.5);
    port("Ubottom")->equals(7.5);
    port("specificHeatCapacity")->equals(42000.);
    port("area")->imports("construction/geometry[groundArea]",CA);
}

void HeatTransferFloor::reset() {
    updateAbsorptivities();
    temperature = 20.;
}

void HeatTransferFloor::update() {
    updateLwEmission();
    lwFluxDown = 0;
    updateTemperature();
}

} //namespace

