/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "screens.h"

using namespace base;

namespace vg {

PUBLISH(Screens)

Screens::Screens(QString name, QObject *parent)
    : Box(name, parent)
{
    help("summarizes attributes of Screen boxes");
    Input(screenAirTransmissivities).imports("./*<Screen>[transmissivityAirNet]");
    Output(airTransmissivity).help("Proportion of air transmitted through the screens").unit("[0;1]");
}

void Screens::reset() {
    update();
}

void Screens::update() {
    airTransmissivity = 1;
    for (double tr : screenAirTransmissivities)
        airTransmissivity *= tr;
}

} //namespace

