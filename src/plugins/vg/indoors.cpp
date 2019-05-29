/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "indoors.h"

using namespace base;

namespace vg {

PUBLISH(Indoors)

Indoors::Indoors(QString name, QObject *parent)
    : Box(name, parent)
{
    help("contains sub-models for indoors climate");
}

void Indoors::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./given"))
        builder.
        box("Given").name("given").
        endbox();

    if (!findMaybeOne<Box>("./controlled"))
        builder.
        box("Controlled").name("controlled").
        endbox();

    if (!findMaybeOne<Box>("./total"))
        builder.
        box().name("total").
            box("vg::Sum").name("airFlux").
                port("inputs").equals("(given/airFlux[value] cooling/airFluxVents[value])").
            endbox().
            box("vg::VapourFluxSum").name("vapourFlux").
                port("toAdd").equals("(given/vapourFlux cooling/vapourFlux)").
            endbox().
            box("vg::Sum").name("energyFlux").
                port("inputs").equals("(given/energyFlux[value] controlled/cooling/energyFlux[value] controlled/heating/energyFlux[value])").
            endbox().
        endbox();

    if (!findMaybeOne<Box>("./light"))
        builder.
        box("vg::IndoorsLight").name("light").
        endbox();
    if (!findMaybeOne<Box>("./temperature"))
        builder.
        box("vg::IndoorsTemperature").name("temperature").
            port("energyFlux").imports("total/energyFlux[value]").
        endbox();
    if (!findMaybeOne<Box>("./humidity"))
        builder.
        box("vg::IndoorsHumidity").name("humidity").
        endbox();
    if (!findMaybeOne<Box>("./co2"))
        builder.
        box("vg::IndoorsCo2").name("co2").
        endbox();
    if (!findMaybeOne<Box>("./windSpeed"))
        builder.
        box("vg::IndoorsWindSpeed").name("windSpeed").
        endbox();
}

} //namespace

