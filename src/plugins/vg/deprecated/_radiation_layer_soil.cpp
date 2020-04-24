/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "radiation_layer_soil.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(RadiationLayerSoil)

RadiationLayerSoil::RadiationLayerSoil(QString name, QObject *parent)
    : RadiationLayer(name, parent)
{
    help("holds the radiative parameters of the floor");
    port("area")->imports("construction/geometry[groundArea]");
    // Default to heat capacity of concrete
    port("specificHeatCapacity")->equals(infinity());
    port("temperature")->imports("outdoors[soilTemperature]");
}

void RadiationLayerSoil::initialize() {
    swTransmissivity  = lwTransmissivity  =
    swReflectivityTop = lwReflectivityTop =
    swReflectivityBottom  = lwReflectivityTop  = 0.;
    updateAbsorptivities();
}

} //namespace

