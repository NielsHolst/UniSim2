/* Copyright 2005-2019 by
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
    Input(coverage).equals(0.9).unit("[0;1]").help("Proportion of floor area with crop");
}

void Crop::amend() {
    BoxBuilder builder(this);

    if (!findMaybeOne<Box>("./rs"))
        builder.
        box("vg::StomatalResistanceRose").name("rs").
        endbox();

    if (!findMaybeOne<Box>("./rb"))
        builder.box("vg::BoundaryLayerResistanceStanghellini").name("rb").
        endbox();

    builder.
    box("vg::LeafTranspiration").name("transpiration").
    endbox().
    box("vg::LeafTemperature").name("temperature").
    endbox().
    box("vg::LeafPhotosynthesis").name("photosynthesis").
    endbox();

    if (!findMaybeOne<Box>("./growth"))
        builder.
        box("vg::CropGrowth").name("growth").
        endbox();
}

} //namespace

