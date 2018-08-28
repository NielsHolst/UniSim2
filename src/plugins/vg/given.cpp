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
    if (!findMaybeOne<Box>("./airFlux"))
        builder.
        box("AirFluxGiven").name("airFlux").
        endbox();

    if (!findMaybeOne<Box>("./vapourFlux"))
        builder.
        box("VapourFluxSum").name("vapourFlux").
            box("VapourFluxTranspiration").name("transpiration").
            endbox().
            box("VapourFluxCondensation").name("condensationCover").
                port("surfaceAreaPerGroundArea").imports("geometry[coverPerGroundArea]").
                port("surfaceTemperature").imports("given/energyFlux/shelter[coverTemperature]").
            endbox().
            box("VapourFluxCondensation").name("condensationScreens").
                port("surfaceAreaPerGroundArea").imports("geometry[coverPerGroundArea]").
                port("surfaceTemperature").imports("given/energyFlux/shelter[screensTemperature]").
            endbox().
            box("VapourFluxAir").name("airFluxOutdoors").
                port("airFlux").imports("given/airFlux[value]").
            endbox().
        endbox();

    if (!findMaybeOne<Box>("./energyFlux"))
        builder.
        box("EnergyFluxSum").name("energyFlux").
//            box("vg::Sum").name("light").
//                port("inputs").imports("indoors/light[total]").  // simplify?
//            endbox().
            box("EnergyFluxCondensation").name("condensationCover").
                port("vapourFlux").imports("../../vapourFlux/condensationCover[vapourFlux]").
            endbox().
            box("EnergyFluxCondensation").name("condensationScreens").
                port("vapourFlux").imports("../../vapourFlux/condensationScreens[vapourFlux]").
            endbox().
            box("EnergyFluxAir").name("airFlux").
                port("airFlux").imports("given/airFlux[value]").
            endbox().
            box("Accumulator").name("growthLights").
                port("change").imports("./controller[controlVariable]").
                box("PidController").name("controller").
                    port("Kprop").equals(0.1).
                    port("sensedValue").imports("..[value]").
                    port("desiredValue").imports("actuators/growthLights[energyFlux]").
                endbox().
            endbox().
            box("EnergyFluxShelter").name("shelter").
            endbox().
            box("EnergyFluxFloor").name("floor").
                port("Uindoors").equals(7.5).
                port("Usoil").equals(4).
                port("heatCapacity").equals(42000).
                box("FloorRadiationAbsorbed").name("radiationAbsorbed").
                endbox().
            endbox().
            box("EnergyFluxSunlight").name("sunlight").
            endbox().
            box("EnergyFluxTranspiration").name("transpiration").
            endbox().
        endbox();
}

} //namespace

