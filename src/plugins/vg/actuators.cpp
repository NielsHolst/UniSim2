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

    if (!findMaybeOne<Box>("./vents"))
        builder.
        box("Accumulator").name("vents").
            port("initial").imports("controllers/ventilation/crack[value]").
            port("change").imports("./controller[controlVariable]").
            port("minValue").equals(0.).
            port("maxValue").equals(1.).
            box("PidController").name("controller").
                port("sensedValue").imports("..[value]").
                port("desiredValue").imports("controllers/ventilation[value]").
                port("Kprop").equals(0.1).
                port("Kderiv").equals(0.2).
                port("Kint").equals(0.01).
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
                port("Kprop").equals(0.05).
            endbox().
        endbox();
}

} //namespace

