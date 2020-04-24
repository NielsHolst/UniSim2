/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/dialog.h>
#include <base/box_builder.h>
#include <base/path.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "shelter_face.h"

using namespace base;
using phys_math::div0;

namespace vg {

PUBLISH(ShelterFace)

ShelterFace::ShelterFace(QString name, QObject *parent)
    : ShelterBase(name, parent)
{
    help("models a shelter face");
    Input(roofArea).imports("geometry[roofArea]").unit("m2");
    Input(sideWallsArea).imports("geometry[sideWallsArea]").unit("m2");
    Input(endWallsArea).imports("geometry[endWallsArea]").unit("m2");
    Input(gablesArea).imports("geometry[gablesArea]").unit("m2");
    Input(groundArea).imports("geometry[groundArea]").unit("m2");

    Output(area).help("Surface area").unit("m2");
    Output(relativeArea).help("Proportion of this surface area out of the total surface area").unit("[0;1]");
    Output(areaPerGround).help("Surface area relative to the ground area").unit("m2/m2");
}

void ShelterFace::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box>("./cover"))
        builder.box("Cover").name("cover").endbox();
    if (objectName().contains("roof") && !findMaybeOne<Box>("./vent")) {
        QString name = "vent"; // + objectName().back();
        builder.box("Vent").name(name).endbox();
    }
}

void ShelterFace::initialize() {
    QString name = objectName();
    if (name.toLower().contains("roof"))
        area = roofArea/2;
    else if (name.toLower().contains("side"))
        area = sideWallsArea/2;
    else if (name.toLower().contains("end"))
        area = endWallsArea/2;
    else {
        QString msg{"Cover name is '%1' but it must contain 'roof', 'side' or 'end'"};
        ThrowException(msg.arg(name));
    }
    relativeArea = area/(roofArea + sideWallsArea + endWallsArea);
    areaPerGround = area/groundArea;
}

} //namespace

