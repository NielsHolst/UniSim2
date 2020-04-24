/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "radiation_layer_floor.h"

using namespace base;

namespace vg {

PUBLISH(RadiationLayerFloor)

RadiationLayerFloor::RadiationLayerFloor(QString name, QObject *parent)
    : RadiationLayer(name, parent)
{
    help("holds the radiative parameters of the floor");
    port("area")->imports("construction/geometry[groundArea]",CA);
}

void RadiationLayerFloor::initialize() {
    swTransmissivityTop  = lwTransmissivityBottom  = 0.;
    swReflectivityTop = lwReflectivityTop = 0.6;
    swReflectivityBottom  = lwReflectivityTop  = 0.;
    updateAbsorptivities();

    Utop = 7.5;
    Ubottom = 7.5;
    specificHeatCapacity = 42000.; // concrete
    temperature = 20.;
}

} //namespace

