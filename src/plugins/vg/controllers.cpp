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
    : Box(name, parent)
{
    help("contains sub-models to control actuators");
}

void Controllers::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./screens"))
        builder.
        box().name("screens").
            box("Accumulator").name("maxDrawn").
                port("initial").equals(1).
                port("change").imports("./controller[controlVariable]").
                box("PidController").name("controller").
                    port("sensedValue").imports("..[value]").
                    port("desiredValue").imports("./target[signal]").
                    port("Kprop").equals(0.02).
                    box("vg::ThresholdSignal").name("target").
                        port("input").imports("indoors/humidity[rh]").
                        port("threshold").imports("setpoints/humidity/maximumRh[value]").
                        port("signalBelow").equals(1).
                        port("signalAbove").equals(0.9).
                    endbox().
                endbox().
            endbox().
            box("Minimum").name("energy").
                port("values").imports("./signals/*[signal]").
                box().name("signals").
                    box("vg::ThresholdSignal").name("radiation").
                        port("input").imports("outdoors[radiation]").
                        port("threshold").equals(10).
                        port("signalBelow").imports("controllers/screens/maxDrawn[value]").
                        port("signalAbove").equals(0).
                    endbox().
                    box("vg::ThresholdSignal").name("temperature").
                        port("input").imports("indoors/temperature[value]").
                        port("threshold").imports("setpoints/temperature/ventilation[value]").
                        port("signalBelow").imports("controllers/screens/maxDrawn[value]").
                        port("signalAbove").equals(0).
                    endbox().
                endbox().
            endbox().
            box("Maximum").name("shade").
                port("values").imports("./signals/*[signal]").
                box().name("signals").
                    box("vg::ThresholdSignal").name("radiation").
                        port("input").imports("outdoors[radiation]").
                        port("threshold").equals(500).
                        port("signalBelow").equals(0).
                        port("signalAbove").imports("controllers/screens/maxDrawn[value]").
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
                        port("beginTime").imports("calendar[sunrise]").
                        port("endTime").imports("calendar[sunset]").
                        port("signalInside").equals(1).
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
            box("vg::AnyFlag").name("on").
                box("vg::DateTimeSignal").
                    port("beginDay").equals(1).
                    port("endDay").equals(365).
                    port("beginTime").equals(QTime(0,0,0)).
                    port("endTime").equals(QTime(23,59,0)).
                    port("circadian").equals(true).
                    port("signalInside").imports("./on[signal]").
                    port("signalOutside").equals(0).
                    port("signalOutsideTimeOnly").equals(0).
                    box("vg::ThresholdSignal").name("on").
                        port("input").imports("outdoors[radiation]").
                        port("threshold").equals(40).
                        port("signalBelow").equals(1).
                        port("signalAbove").equals(0).
                    endbox().
                endbox().
            endbox().
            box("vg::AnyFlag").name("off").
                box("vg::DateTimeSignal").
                    port("beginDay").equals(1).
                    port("endDay").equals(365).
                    port("beginTime").equals(QTime(0,0,0)).
                    port("endTime").equals(QTime(23,59,0)).
                    port("circadian").equals(true).
                    port("signalInside").imports("./off[signal]").
                    port("signalOutside").equals(0).
                    port("signalOutsideTimeOnly").equals(0).
                    box("vg::ThresholdSignal").name("off").
                        port("input").imports("outdoors[radiation]").
                        port("threshold").equals(60).
                        port("signalBelow").equals(0).
                        port("signalAbove").equals(1).
                    endbox().
                endbox().
            endbox().
            box("vg::AnyFlag").name("periods").
                box("vg::DateTimeSignal").
                    port("beginDay").equals(1).
                    port("endDay").equals(365).
                    port("beginTime").imports("calendar[sunrise]").
                    port("endTime").imports("calendar[sunset]").
                    port("circadian").equals(true).
                    port("signalInside").equals(1).
                    port("signalOutside").equals(0).
                    port("signalOutsideTimeOnly").equals(0).
                endbox().
            endbox().
        endbox();

    if (!findMaybeOne<Box>("./co2"))
        builder.
        box("vg::Co2Controller").name("co2").
            port("injectionRate").imports("./injectionRate[signal]").
            box("vg::ProportionalSignal").name("injectionRate").
                port("input").imports("indoors/total/airFlux[value]").
                port("threshold").equals(6).
                port("thresholdBand").equals(1).
                port("increasingSignal").equals(false).
                port("minSignal").equals(0).
                port("maxSignal").equals(4.5).
            endbox().
        endbox();

}

} //namespace

