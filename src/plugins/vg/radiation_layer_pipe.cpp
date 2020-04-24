/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "radiation_layer_pipe.h"

using namespace base;

namespace vg {

PUBLISH(RadiationLayerPipe)

RadiationLayerPipe::RadiationLayerPipe(QString name, QObject *parent)
    : RadiationLayer(name, parent)
{
    help("holds the radiative parameters of the heat pipes");
    Input(energyFlux).imports("heating/energyFlux[energyFlux]").transform(Sum);
    port("area")->imports("construction/geometry[groundArea]",CA);
}

void RadiationLayerPipe::initialize() {
    temperature = phys_math::infinity();
    swReflectivityBottom = swReflectivityTop =
    lwReflectivityBottom = lwReflectivityTop = 0.;
    swTransmissivityTop = swTransmissivityBottom =
    lwTransmissivityTop = lwTransmissivityBottom = 1.;
    updateAbsorptivities();
}

void RadiationLayerPipe::reset() {
    update();
}

void RadiationLayerPipe::update() {
    // Spread the flux equally upwards and downwards
    lwFluxDown = lwFluxUp = energyFlux/2.;
}

} //namespace

