/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
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

//    if (!findMaybeOne<Box>("./heating"))
//        builder.
//        box().name("heating").
//            newPort("value").imports("./controller[controlVariable]",CA).
//            box("PidController").name("controller").
//                port("sensedValue").imports("indoors[temperature]",CA).
//                port("desiredValue").imports("setpoints/heatingTemperature[value]",CA).
//                port("desire").equals("KeepAbove").
//                port("minimum").equals(0).
//                port("maximum").equals(1).
//                port("Kprop").equals(0.05).
//                port("Kderiv").equals(-0.5).
//            endbox().
//        endbox();

//    if (!findMaybeOne<Box>("./ventilation"))
//        builder.
//        box("Maximum").name("ventilation").
//            port("values").imports("./controller[controlVariable] | setpoints[crackVentilation]").
//            box("PidController").name("controller").
//                port("sensedValue").imports("indoors[temperature]",CA).
//                port("desiredValue").imports("setpoints/ventilationTemperature[value]",CA).
//                port("desire").equals("KeepBelow").
//                port("minimum").equals(0).
//                port("maximum").equals(1).
//                port("Kprop").equals(0.05).
//                port("Kderiv").equals(-0.5).
//            endbox().
//        endbox();

    if (!findMaybeOne<Box>("./screens"))
        builder.
        box().name("screens").
            box("ProportionalSignal").name("energy1").
                port("increasingSignal").equals(false).
                port("maxSignal").equals(1).
                port("input").imports("outdoors[radiation]").
                port("threshold").imports("setpoints[screenEnergyThreshold1]").
                port("thresholdBand").imports("setpoints[screenEnergyThresholdBand]").
            endbox().
            box("ProportionalSignal").name("energy2").
                port("increasingSignal").equals(false).
                port("maxSignal").equals(1).
                port("input").imports("outdoors[radiation]").
                port("threshold").imports("setpoints[screenEnergyThreshold2]").
                port("thresholdBand").imports("setpoints[screenEnergyThresholdBand]").
            endbox().
            box("ProportionalSignal").name("shade1").
                port("increasingSignal").equals(false).
                port("maxSignal").equals(1).
                port("input").imports("outdoors[radiation]").
                port("threshold").imports("setpoints[screenShadeThreshold1]").
                port("thresholdBand").imports("setpoints[screenShadeThresholdBand]").
            endbox().
            box("ProportionalSignal").name("shade2").
                port("increasingSignal").equals(false).
                port("maxSignal").equals(1).
                port("input").imports("outdoors[radiation]").
                port("threshold").imports("setpoints[screenShadeThreshold2]").
                port("thresholdBand").imports("setpoints[screenShadeThresholdBand]").
            endbox().
            box().name("fixed1").
                newPort("value").imports("setpoints[screenFixed1]").
            endbox().
        endbox();

    if (!findMaybeOne<Box>("./co2Capacity"))
        builder.
        box("ProportionalSignal").name("co2Capacity").
            port("input").imports("actuators/ventilation[value]").
            port("threshold").imports("setpoints[co2VentilationThreshold]").
            port("thresholdBand").imports("setpoints[co2VentilationBand]").
            port("maxSignal").imports("setpoints[co2Capacity]").
            port("minSignal").equals(0).
            port("increasingSignal").equals(false).
        endbox();

    if (!findMaybeOne<Box>("./growthLights"))
        builder.
        box().name("growthLights").
            box("GrowthLightController").name("growthLight1").
                port("setting").imports("setpoints[growthLightSetting1]").
                port("lightThresholdLow").imports("setpoints[growthLightThresholdLow1]").
                port("lightThresholdHigh").imports("setpoints[growthLightThresholdHigh1]").
            endbox().
            box("GrowthLightController").name("growthLight2").
                port("setting").imports("setpoints[growthLightSetting2]").
                port("lightThresholdLow").imports("setpoints[growthLightThresholdLow2]").
                port("lightThresholdHigh").imports("setpoints[growthLightThresholdHigh2]").
            endbox().
            box("GrowthLightController").name("growthLight3").
                port("setting").imports("setpoints[growthLightSetting3]").
                port("lightThresholdLow").imports("setpoints[growthLightThresholdLow3]").
                port("lightThresholdHigh").imports("setpoints[growthLightThresholdHigh3]").
            endbox().
        endbox();

}

} //namespace

