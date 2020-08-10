/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "given.h"

using namespace base;

namespace vg {

PUBLISH(Given)

Given::Given(QString name, QObject *parent)
    : Box(name, parent)
{
    help("contains given fluxes of air, water vapour and energy");
}

void Given::amend() {
    BoxBuilder builder(this);
    amendAirFlux(builder);
    amendVapourFlux(builder);
    amendEnergyFlux(builder);
}

void Given::amendAirFlux(BoxBuilder &builder) {
    if (!findMaybeOne<Box>("./airFlux"))
        builder.
        box("AirFluxGiven").name("airFlux").
        endbox();
}

void Given::amendVapourFlux(BoxBuilder &builder) {
    Box *vapourFlux = findMaybeOne<Box>("./vapourFlux");
    bool createVapourFlux = !vapourFlux;

    // Either create vapourFlux box or move to existing vapourFlux box
    if (createVapourFlux)
        builder.
        box("VapourFluxSum").name("vapourFlux");
    else
        builder.moveToBox(vapourFlux);

    // Create missing boxes inside vapourFlux
    if (!findMaybeOne<Box>("./vapourFlux/transpiration"))
        builder.
        box("VapourFluxTranspiration").name("transpiration").
        endbox();
    if (!findMaybeOne<Box>("./vapourFlux/condensationCover"))
        builder.
        box("VapourFluxCondensation").name("condensationCover").
            port("surfaceAreaPerGroundArea").imports("geometry[coverPerGroundArea]",CA).
            port("surfaceTemperature").imports("given/energyFlux/shelter[coverTemperature]",CA).
        endbox();
    if (!findMaybeOne<Box>("./vapourFlux/condensationScreens"))
        builder.
        box("VapourFluxCondensation").name("condensationScreens").
            port("surfaceAreaPerGroundArea").imports("geometry[coverPerGroundArea]",CA).
            port("surfaceTemperature").imports("given/energyFlux/shelter[screensTemperature]",CA).
        endbox();
    if (!findMaybeOne<Box>("./vapourFlux/airFluxOutdoors"))
        builder.
        box("VapourFluxAir").name("airFluxOutdoors").
            port("airFlux").imports("given/airFlux[value]",CA).
        endbox();

    // Finish creation of vapourFlux box
    if (createVapourFlux)
        builder.
        endbox();
}

void Given::amendEnergyFlux(BoxBuilder &builder) {
    if (!findMaybeOne<Box>("./energyFlux"))
        builder.
        box("RadiationLayers").name("energyFlux").
        endbox();
}

} //namespace

