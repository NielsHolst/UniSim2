/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "setpoints.h"

using namespace base;

namespace vg {

PUBLISH(Setpoints)

Setpoints::Setpoints(QString name, QObject *parent)
    : SetpointsInput(name, parent)
{
    help("contains sub-models for setpoints");
}

void Setpoints::amend() {
    BoxBuilder builder(this);

//    if (!findMaybeOne<Box>("./input"))
//        builder.
//        box("SetpointsInput").name("input");

    if (!findMaybeOne<Box>("./daylightLevel"))
        builder.
        box("vg::DaylightLevel").name("daylightLevel").
            port("dawnThreshold").imports("setpoints[dawnThreshold]",CA).
            port("duskThreshold").imports("setpoints[duskThreshold]",CA).
        endbox();

    if (!findMaybeOne<Box>("./temperature"))
        builder.
        box().name("temperature").
        box("Accumulator").name("ventilation").
            port("initial").imports("./controller/target[maxSignal]").
            port("change").imports("./controller[controlVariable]").
            box("PidController").name("controller").
                port("desiredValue").imports("./target[signal]").
                port("sensedValue").imports("..[value]").
                port("Kprop").equals(0.1).
//                port("Kint").equals(0.001).
                box("ProportionalSignal").name("target").
                    port("input").imports("indoors/humidity[rh]",CA).
                    port("threshold").imports("setpoints[rhMax]",CA).
                    port("thresholdBand").imports("setpoints[rhMaxBand]",CA).
                    port("increasingSignal").equals(false).
                    port("minSignal").imports("setpoints[ventilationTemperatureAtHighRh]",CA).
                    port("maxSignal").imports("setpoints[ventilationTemperatureAtLowRh]",CA).
                endbox().
            endbox().
        endbox().
        box("Accumulator").name("heating").
            port("initial").imports("./controller/target[maxSignal]").
            port("change").imports("./controller[controlVariable]").
            box("PidController").name("controller").
                port("desiredValue").imports("./target[signal]").
                port("sensedValue").imports("..[value]").
                port("Kprop").equals(0.1).
                box("ProportionalSignal").name("target").
                    port("input").imports("indoors/humidity[rh]",CA).
                    port("threshold").imports("setpoints[rhMax]",CA).
                    port("thresholdBand").imports("setpoints[rhMaxBand],CA").
                    port("increasingSignal").equals(true).
                    port("minSignal").imports("setpoints[heatingTemperatureAtHighRh]",CA).
                    port("maxSignal").imports("setpoints[heatingTemperatureAtLowRh]",CA).
                endbox().
            endbox().
        endbox().
    endbox();
}

} //namespace

