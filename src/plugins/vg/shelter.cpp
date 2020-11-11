/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "shelter.h"
#include <base/vector_op.h>

using std::max;
using namespace base;

namespace vg {

PUBLISH(Shelter)

Shelter::Shelter(QString name, QObject *parent)
    : Box(name, parent)
{
    help("models the greenhouse shelter");
    Input(airTransmissivities).imports("./*/screens/airTransmissivity[value]", CA).
            help("Air transmissivity through screens for each shelter face");
    Input(areas).imports("./*/area[value]", CA).
            help("Area of each shelter face");
    Output(screensAirTransmissivity).help("Total net transmissivity of screens").unit("[0;1]");
}

void Shelter::amend() {
    BoxBuilder builder(this);
    amendShelter(builder, "roof1");
    amendShelter(builder, "roof2");
    amendShelter(builder, "side1");
    amendShelter(builder, "side2");
    amendShelter(builder, "end1");
    amendShelter(builder, "end2");
}

void Shelter::amendShelter(BoxBuilder &builder, QString shelterName) {
    if (!findMaybeOne<Box>("./" + shelterName))
        builder.box("ShelterFace").name(shelterName).endbox();
}

void Shelter::update() {
    screensAirTransmissivity = vector_op::weightedAverage(airTransmissivities, areas, this);
}

} //namespace

