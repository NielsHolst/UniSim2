/* Copyright 2005-2018 byoC
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
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
            port("dawnThreshold").imports("setpoints[dawnThreshold]").
            port("duskThreshold").imports("setpoints[duskThreshold]").
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
                box("vg::ProportionalSignal").name("target").
                    port("input").imports("indoors/humidity[rh]").
                    port("threshold").imports("setpoints[rhMax]").
                    port("thresholdBand").imports("setpoints[rhMaxBand]").
                    port("increasingSignal").equals(false).
                    port("minSignal").imports("setpoints[ventilationTemperatureAtHighRh]").
                    port("maxSignal").imports("setpoints[ventilationTemperatureAtLowRh]").
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
                box("vg::ProportionalSignal").name("target").
                    port("input").imports("indoors/humidity[rh]").
                    port("threshold").imports("setpoints[rhMax]").
                    port("thresholdBand").imports("setpoints[rhMaxBand]").
                    port("increasingSignal").equals(true).
                    port("minSignal").imports("setpoints[heatingTemperatureAtHighRh]").
                    port("maxSignal").imports("setpoints[heatingTemperatureAtLowRh]").
                endbox().
            endbox().
        endbox().
    endbox();

    if (!findMaybeOne<Box>("./co2"))
      builder.
     box().name("co2").
         box().name("minimum").
             newPort("signal").equals(650.).
         endbox().
         box().name("maximum").
             newPort("signal").equals(650.).
         endbox().
     endbox();
}

} //namespace

