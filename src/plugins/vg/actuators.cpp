/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "actuators.h"

using namespace base;

namespace vg {

PUBLISH(Actuators)

Actuators::Actuators(QString name, QObject *parent)
    : Box(name, parent)
{
    help("contains sub-models for actuators");
}

void Actuators::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./screens")) {
        builder.
        box().name("screens").
        endbox();
        amendScreen("energy");
        amendScreen("shade");
        amendScreen("blackout");
    }

    if (!findMaybeOne<Box>("./heating"))
        builder.
        box().name("heating").
            box().name("pipes").
                box("vg::Pipe").
                    port("length").equals(1.8).
                    port("diameter").equals(52).
                    port("minTemperature").equals(20).
                    port("maxTemperature").equals(80).
                endbox().
            endbox().
        endbox();

    if (!findMaybeOne<Box>("./growthLights"))
        builder.
        box("vg::GrowthLights").name("growthLights").
        endbox();
}

void Actuators::amendScreen(QString name) {
    BoxBuilder builder(findOne<Box>("./screens"));
    builder.
        box("Accumulator").name(name).
            port("change").imports("./controller[controlVariable]").
            box("PidController").name("controller").
                port("sensedValue").imports("..[value]").
                port("desiredValue").imports("controllers/screens/"+name+"[value]").
                port("Kprop").equals(0.3).
            endbox().
        endbox();
}

} //namespace

