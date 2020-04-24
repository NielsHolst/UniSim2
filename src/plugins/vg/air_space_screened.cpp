/* Copyright 2005-2019 by
 *
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "air_space_screened.h"

using namespace base;

namespace vg {

PUBLISH(AirSpaceScreened)

AirSpaceScreened::AirSpaceScreened(QString name, QObject *parent)
    : AirSpaceBase(name, parent)
{
    Input(area).imports("..[effectiveArea]").unit("m2 surface").help("Area air space");
    Input(depth).equals(0.1).help("Depth of air space").unit("m");
    Input(roofHeight).imports("construction/geometry[roofHeight]",CA);
}

void AirSpaceScreened::initialize() {
    // First air space below the roof always uses half of the roof height for its depth
    // (thus assuming a horizontal top screen)
    const Box *screen = findOne<Box>(".."),
              *face = findOne<Box>("../../..");
    if (screen->name()=="layer1" && face->name().contains("roof"))
        depth = roofHeight/2;
}

void AirSpaceScreened::updateVolume() {
    volume = area*depth;
}

} //namespace

