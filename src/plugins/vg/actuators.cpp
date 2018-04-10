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
    if (!findMaybeOne<Box>("./screens"))
        builder.
        box().name("screens").
            box().name("energy").
            endbox().
            box().name("shade").
            endbox().
            box().name("blackout").
            endbox().
        endbox();

    if (!findMaybeOne<Box>("./heating"))
        builder.
        box().name("heating").
            box().name("pipes").
            endbox().
        endbox();

    if (!findMaybeOne<Box>("./growthLights"))
        builder.
        box("vg::GrowthLights").name("growthLights").
        endbox();
}

} //namespace

