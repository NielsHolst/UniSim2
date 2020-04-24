/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "air_space_base.h"

using namespace base;

namespace vg {

AirSpaceBase::AirSpaceBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(temperature).equals(20.).help("Air temperature").unit("oC");
    Output(volume).help("Air space volume").unit("m3");
}

void AirSpaceBase::reset() {
    update();
}

void AirSpaceBase::update() {
    updateVolume();
}

} //namespace

