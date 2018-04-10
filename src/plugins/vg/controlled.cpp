/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "controlled.h"

using namespace base;

namespace vg {

PUBLISH(Controlled)

Controlled::Controlled(QString name, QObject *parent)
    : Box(name, parent)
{
    help("contains given fluxes of air, water vapour and energy");
}

void Controlled::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./energyFlux"))
        builder.
        box().name("energyFlux").
            box().name("heating").
                box("vg::Sum").name("minSupply").
                    port("inputs").equals("actuators/heating/pipes/*[nextEnergyFluxMin]").
                endbox().
                box("vg::Sum").name("maxSupply").
                    port("inputs").equals("actuators/heating/pipes/*[nextEnergyFluxMax]").
                endbox().
                box("vg::EnergyFluxHeatingDemand").name("demand").
                endbox().
                box("Accumulator").name("supply").
                    port("change").imports("./controller[controlVariable]").
                    port("minValue").imports("../minSupply[value]").
                    port("maxValue").imports("../maxSupply[value]").
                    box("PidController").name("controller").
                        port("sensedValue").imports("..[value]").
                        port("desiredValue").imports("../../demand[value]").
                        port("Kprop").equals(0.6).
                        port("Kint").equals(0.01).
                    endbox().
                endbox().
            endbox().
            box("IndoorsTemperature").name("temperature").
                port("energyFlux").imports("./energyFlux[value]").
                port("baseTemperature").imports("indoors/temperature[value]").
                box("vg::Sum").name("energyFlux").
                    port("inputs").equals("(given/energyFlux[value] energyFlux/heating/supply[value])").
                endbox().
            endbox().
            box().name("cooling").
                box("vg::EnergyFluxCoolingDemand").name("demand").
                endbox().
                box("vg::AirFluxCoolingSupplyMax").name("airSupplyMax").
                    box("vg::VentilationByWind").name("byWind").
                    endbox().
                    box("vg::VentilationByTemperature").name("byTemperature").
                    endbox().
                endbox().
                box("Accumulator").name("supply").
                    port("change").imports("./controller[controlVariable]").
                    box("PidController").name("controller").
                        port("desiredValue").imports("./target[value]").
                        port("sensedValue").imports("..[value]").
                        port("Kprop").equals(0.5).
                        port("maximum").equals(0).
                        port("maxSlope").equals(1).
                        port("minSlope").equals(-1).
                        box("vg::EnergyFluxCoolingSupply").name("target").
                            port("airSupplyMax").imports("cooling/airSupplyMax[value]").
                        endbox().
                    endbox().
                endbox().
            endbox().
        endbox();

    if (!findMaybeOne<Box>("./cooling"))
        builder.
        box().name("cooling").
            box("vg::AirFluxCoolingSupply").name("airFlux").
            endbox().
            box("vg::VapourFluxAir").name("vapourFlux").
                port("airFlux").imports("cooling/airFlux[value]").
            endbox().
            box("vg::EnergyFluxAir").name("energyFlux").
                port("airFlux").imports("cooling/airFlux[value]").
            endbox().
        endbox();
}

} //namespace

