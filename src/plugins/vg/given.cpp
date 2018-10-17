/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
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
            port("surfaceAreaPerGroundArea").imports("geometry[coverPerGroundArea]").
            port("surfaceTemperature").imports("given/energyFlux/shelter[coverTemperature]").
        endbox();
    if (!findMaybeOne<Box>("./vapourFlux/condensationScreens"))
        builder.
        box("VapourFluxCondensation").name("condensationScreens").
            port("surfaceAreaPerGroundArea").imports("geometry[coverPerGroundArea]").
            port("surfaceTemperature").imports("given/energyFlux/shelter[screensTemperature]").
        endbox();
    if (!findMaybeOne<Box>("./vapourFlux/airFluxOutdoors"))
        builder.
        box("VapourFluxAir").name("airFluxOutdoors").
            port("airFlux").imports("given/airFlux[value]").
        endbox();

    // Finish creation of vapourFlux box
    if (createVapourFlux)
        builder.
        endbox();
}

void Given::amendEnergyFlux(BoxBuilder &builder) {
    Box *energyFlux = findMaybeOne<Box>("./energyFlux");
    bool createEnergyFlux = !energyFlux;

    // Either create vapourFlux box or move to existing vapourFlux box
    if (createEnergyFlux)
        builder.
        box("EnergyFluxSum").name("energyFlux");
    else
        builder.moveToBox(energyFlux);

    // Create missing boxes inside energyFlux
    if (!findMaybeOne<Box>("./energyFlux/condensationCover"))
        builder.
        box("EnergyFluxCondensation").name("condensationCover").
            port("vapourFlux").imports("../../vapourFlux/condensationCover[vapourFlux]").
        endbox();
    if (!findMaybeOne<Box>("./energyFlux/condensationScreens"))
        builder.
        box("EnergyFluxCondensation").name("condensationScreens").
            port("vapourFlux").imports("../../vapourFlux/condensationScreens[vapourFlux]").
        endbox();
    if (!findMaybeOne<Box>("./energyFlux/airFlux"))
        builder.
        box("EnergyFluxAir").name("airFlux").
            port("airFlux").imports("given/airFlux[value]").
        endbox();
    if (!findMaybeOne<Box>("./energyFlux/growthLights"))
        builder.
        box("Accumulator").name("growthLights").
            port("change").imports("./controller[controlVariable]").
            box("PidController").name("controller").
                port("Kprop").equals(0.1).
                port("sensedValue").imports("..[value]").
                port("desiredValue").imports("actuators/growthLights[powerUsage]").
            endbox().
        endbox();
    if (!findMaybeOne<Box>("./energyFlux/shelter"))
        builder.
        box("EnergyFluxShelter").name("shelter").
        endbox();
    if (!findMaybeOne<Box>("./energyFlux/floor"))
        builder.
        box("EnergyFluxFloor").name("floor").
            port("Uindoors").equals(7.5).
            port("Usoil").equals(4).
            port("heatCapacity").equals(42000).
            box("FloorRadiationAbsorbed").name("radiationAbsorbed").
            endbox().
        endbox();
    if (!findMaybeOne<Box>("./energyFlux/sunlight"))
        builder.
        box("EnergyFluxSunlight").name("sunlight").
        endbox();
    if (!findMaybeOne<Box>("./energyFlux/transpiration"))
        builder.
        box("EnergyFluxTranspiration").name("transpiration").
        endbox();

    // Finish creation of vapourFlux box
    if (createEnergyFlux)
        builder.
        endbox();
}

} //namespace

