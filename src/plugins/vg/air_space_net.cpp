/* Copyright 2005-2019 by
 *
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "air_space_net.h"

using namespace base;

namespace vg {

PUBLISH(AirSpaceNet)

AirSpaceNet::AirSpaceNet(QString name, QObject *parent)
    : AirSpaceBase(name, parent)
{
    Input(indoorsVolumeTotal).imports("construction/geometry[indoorsVolumeTotal]",CA);
    Input(screenedVolumes).imports("../shelter/*/*/*/airSpace[volume]");
    Output(screenedVolumeTotal).help("Total volume behind screens").unit("m3");
}

void AirSpaceNet::updateVolume() {
    screenedVolumeTotal = vector_op::sum(screenedVolumes);
    volume = indoorsVolumeTotal - screenedVolumeTotal ;
}

} //namespace

