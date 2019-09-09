/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <math.h>
#include <base/box_builder.h>
#include <base/publish.h>
#include "air_flux_given.h"

using namespace base;

namespace vg {

PUBLISH(AirFluxGiven)

AirFluxGiven::AirFluxGiven(QString name, QObject *parent)
    : AirFluxBase(name, parent)
{
    help("sums up total given air flux");
    Input(airFluxes).imports("./*[value]").unit("m3/m3/h");;
}

void AirFluxGiven::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./infiltration"))
        builder.
        box("AirFluxInfiltration").name("infiltration").
        endbox();

//    if (!findMaybeOne<Box>("./gravitation"))
//        builder.
//        box("AirFluxGravitation").name("gravitation").
//        endbox();

//    if (!findMaybeOne<Box>("./crackVentilation"))
//        builder.
//        box("Accumulator").name("crackVentilation").
//            port("change").imports("./controller[controlVariable]").
//            box("PidController").name("controller").
//                port("desiredValue").imports("./desiredVentilation[signal]").
//                port("sensedValue").imports("..[value]").
//                port("Kprop").equals(0.02).
//                box("ProportionalSignal").name("desiredVentilation").
//                    port("input").imports("indoors/humidity[rh]").
//                    port("threshold").imports("setpoints/humidity/maximumRh[value]").
//                    port("thresholdBand").equals(10).
//                    port("increasingSignal").equals(true).
//                    port("maxSignal").imports("./maxVentilation[signal]").
//                    port("minSignal").equals(0).
//                    box("ProportionalSignal").name("maxVentilation").
//                        port("input").imports("outdoors[temperature]").
//                        port("threshold").equals(-5).
//                        port("thresholdBand").equals(1).
//                        port("increasingSignal").equals(true).
//                        port("maxSignal").equals(0.5).
//                        port("minSignal").equals(0).
//                    endbox().
//                endbox().
//            endbox().
//        endbox();
}

void AirFluxGiven::update() {
    value = 0;
    for (double airFlux : airFluxes)
        value += airFlux*airFlux;
    value = std::sqrt(value);
}

} //namespace

