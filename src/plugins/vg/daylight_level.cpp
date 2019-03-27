/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "daylight_level.h"

using namespace base;

namespace vg {

PUBLISH(DaylightLevel)

DaylightLevel::DaylightLevel(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes if its day or night based on sunlight intensity");
    Input(dawnThreshold).equals(0.2).help("Day begins above this sunlight intensity [W/m2]").unit("W/m2");
    Input(duskThreshold).equals(0.3).help("Night begins below this sunlight intensity [W/m2]").unit("W/m2");
    Input(outdoorsRadiation).imports("outdoors[radiation]").unit("W/m2");
    Input(time).imports("calendar[time]").unit("Time");
    Output(isDay).help("Is it day time?").unit("y|n");
    Output(isNight).help("Is it night time?").unit("y|n");
    Output(day).help("Day value").unit("0|1");
    Output(night).help("Night value").unit("0|1");
}

void DaylightLevel::reset() {
    update();
}

void DaylightLevel::update() {
    if (outdoorsRadiation>dawnThreshold && time.hour()<=12) {
        isNight = false; night = 0;
        isDay = true; day = 1;
    }
    if (outdoorsRadiation<duskThreshold && time.hour()>12) {
        isNight = true; night = 1;
        isDay = false; day = 0;
    }
}


} //namespace

