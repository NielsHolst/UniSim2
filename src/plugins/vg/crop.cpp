/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "crop.h"
#include <base/box_builder.h>
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(Crop)

Crop::Crop(QString name, QObject *parent)
	: Box(name, parent)
{
    help("holds crop parameters");
    Input(lai).equals(3.).unit("m2/m2").help("Leaf area index");
    Input(coverage).equals(1.0).unit("[0;1]").help("Proportion of floor area with crop");
}

void Crop::amend() {
    BoxBuilder builder(this);

    if (!findMaybeOne<Box>("./leafWindSpeed"))
        builder.box("vg::LeafWindSpeed").name("leafWindSpeed").
        endbox();
    if (!findMaybeOne<Box>("./boundaryLayerResistance"))
        builder.box("vg::BoundaryLayerResistanceStanghellini").name("boundaryLayerResistance").
        endbox();
    if (!findMaybeOne<Box>("./transpiration"))
        builder.box("vg::LeafTranspiration").name("transpiration").
        endbox();
    if (!findMaybeOne<Box>("./temperature"))
        builder.box("vg::LeafTemperature").name("temperature").
        endbox();
    if (!findMaybeOne<Box>("./photosynthesis"))
        builder.box("vg::CanopyPhotosynthesis").name("photosynthesis").
        endbox();
    if (!findMaybeOne<Box>("./growth"))
        builder.
        box("vg::CropGrowth").name("growth").
        endbox();
}

} //namespace

