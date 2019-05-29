/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "controllers.h"

using namespace base;

namespace vg {

PUBLISH(Controllers)

Controllers::Controllers(QString name, QObject *parent)
    : ControllersInput(name, parent)
{
    help("contains sub-models to control actuators");
}

void Controllers::amend() {
    BoxBuilder builder(this);

//    if (!findMaybeOne<Box>("./input"))
//        builder.
//        box("ControllersInput").name("input");

    if (!findMaybeOne<Box>("./ventilation"))
        builder.
        box("ProportionalSignal").name("ventilation").
            port("input").imports("indoors/temperature[value]").
            port("threshold").imports("setpoints/temperature/ventilation[value]").
            port("thresholdBand").imports("controllers[ventilationThresholdBand]").
            port("maxSignal").equals(1.).
            port("minSignal").imports("./crack[value]").
            box("ProportionalSignal").name("crack").
                port("input").imports("indoors/humidity[rh]").
                port("threshold").imports("setpoints[rhMax]").
                port("thresholdBand").imports("setpoints[rhMaxBand]").
                port("maxSignal").imports("./minsMax[value]").
                port("minSignal").equals(0).
                box("ProportionalSignal").name("minsMax").
                    port("input").imports("outdoors[temperature]").
                    port("threshold").imports("controllers[crackVentilationTemperatureMin]").
                    port("thresholdBand").imports("controllers[crackVentilationTemperatureMinBand]").
                    port("maxSignal").imports("controllers[crackVentilation]").
                    port("minSignal").equals(0).
                endbox().
            endbox().
        endbox();

    if (!findMaybeOne<Box>("./heating"))
        builder.
        box("Accumulator").name("heating").
            port("change").imports("./controller[controlVariable]").
            port("minValue").equals(0).
            port("maxValue").equals(1).
            box("PidController").name("controller").
                port("sensedValue").imports("indoors/temperature[value]").
                port("desiredValue").imports("setpoints/temperature/heating[value]").
                port("Kprop").equals(0.1).
            endbox().
        endbox();

    if (!findMaybeOne<Box>("./screens"))
        builder.
        box().name("screens").
            box("Accumulator").name("maxDrawn").
                port("initial").equals(1).
                port("change").imports("./controller[controlVariable]").
                port("minValue").equals(0.).
                port("maxValue").equals(1.).
                box("PidController").name("controller").
                    port("sensedValue").imports("..[value]").
                    port("desiredValue").imports("./target[signal]").
                    port("Kprop").equals(0.02).
                    box("vg::ThresholdSignal").name("target").
                        port("input").imports("indoors/humidity[rh]").
                        port("threshold").imports("setpoints[rhMax]").
                        port("signalBelow").equals(1).
                        port("signalAbove").imports("controllers[screenMaxAtHighRh]").
                    endbox().
                endbox().
            endbox().
            box("Minimum").name("energy").
                port("values").imports("./signals/*[signal]").
                box().name("signals").
                    box("vg::ProportionalSignal").name("radiation").
                        port("input").imports("outdoors[radiation]").
                        port("threshold").imports("controllers[screenEnergyThreshold]").
                        port("thresholdBand").imports("controllers[screenEnergyThresholdBand]").
                        port("maxSignal").imports("controllers/screens/maxDrawn[value]").
                        port("minSignal").equals(0).
                        port("increasingSignal").equals(false).
                    endbox().
                endbox().
            endbox().
            box("Maximum").name("shade").
                port("values").imports("./signals/*[signal]").
                box().name("signals").
                    box("vg::ProportionalSignal").name("radiation").
                        port("input").imports("outdoors[radiation]").
                        port("threshold").imports("controllers[screenShadeThreshold]").
                        port("thresholdBand").imports("controllers[screenShadeThresholdBand]").
                        port("maxSignal").imports("controllers/screens/maxDrawn[value]").
                        port("minSignal").equals(0).
                    endbox().
                    box().name("asEnergy").
                        newPort("signal").imports("controllers/screens/energy[value]").
                    endbox().
                endbox().
            endbox().
            box("Maximum").name("blackout").
                port("values").imports("./signals/*[signal]").
                box().name("signals").
                    box("vg::DateTimeSignal").name("time").
                        port("beginTime").imports("controllers[screenBlackoutFromTime]").
                        port("endTime").imports("controllers[screenBlackoutToTime]").
                        port("signalInside").imports("setpoints/daylightLevel[day]").
                    endbox().
                    box().name("asEnergy").
                        newPort("signal").imports("controllers/screens/energy[value]").
                    endbox().
                endbox().
            endbox().
        endbox();

    if (!findMaybeOne<Box>("./growthLight"))
        builder.
        box("vg::GrowthLightController").name("growthLight").
        endbox();

    if (!findMaybeOne<Box>("./co2"))
        builder.
        box("Accumulator").name("co2").
            port("change").imports("./controller[controlVariable]").
            port("minValue").equals(0).
            port("maxValue").imports("./co2Capacity[value]").
            box("PidController").name("controller").
                port("sensedValue").imports("indoors/co2[value]").
                port("desiredValue").imports("setpoints[co2Setpoint]").
                port("Kprop").equals(0.1).
            endbox().
            box("vg::ProportionalSignal").name("co2Capacity").
                port("input").imports("actuators/vents[value]").
                port("threshold").imports("setpoints[co2VentilationThreshold]").
                port("thresholdBand").imports("setpoints[co2VentilationBand]").
                port("maxSignal").imports("setpoints[co2Capacity]").
                port("minSignal").equals(0).
                port("increasingSignal").equals(false).
            endbox().
        endbox();

}

} //namespace

