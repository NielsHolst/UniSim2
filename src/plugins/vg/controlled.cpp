/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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
//    if (!findMaybeOne<Box>("./heating"))
//        builder.
//        box("EnergyFluxHeating").name("energyFluxHeating").
//        endbox().
//        box().name("heating").
//            box("vg::Sum").name("minSupply").
//                port("inputs").equals("actuators/heating/pipes/*[nextEnergyFluxMin]").
//            endbox().
//            box("vg::Sum").name("maxSupply").
//                port("inputs").equals("actuators/heating/pipes/*[nextEnergyFluxMax]").
//            endbox().
//            box("vg::EnergyFluxHeatingDemand").name("demand").
//            endbox().
//            box("Accumulator").name("supply").
//                port("change").imports("./controller[controlVariable]").
//                port("minValue").imports("../minSupply[value]").
//                port("maxValue").imports("../maxSupply[value]").
//                box("PidController").name("controller").
//                    port("sensedValue").imports("..[value]").
//                    port("desiredValue").imports("../../demand[value]").
//                    port("Kprop").equals(0.6).
//                    port("Kint").equals(0.01).
//                endbox().
//            endbox().
//        endbox();

    if (!findMaybeOne<Box>("./cooling"))
        builder.
        box().name("cooling").
            box("AirFluxVents").name("airFluxVents").
            endbox().
            box("vg::VapourFluxAir").name("vapourFlux").
                port("airFlux").imports("../airFluxVents[value]").
            endbox().
            box("vg::EnergyFluxAir").name("energyFlux").
                port("airFlux").imports("../airFluxVents[value]").
            endbox().
        endbox();

    if (!findMaybeOne<Box>("./heating"))
        builder.
        box().name("heating").
            box("EnergyFluxHeating").name("energyFlux").
            endbox().
        endbox();
}

} //namespace

