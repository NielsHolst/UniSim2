/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/time_with_units.h>
#include "physiological_time.h"

using namespace base;

namespace vg {

PhysiologicalTime::PhysiologicalTime(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(PhysiologicalTime);
    Input(step);
    Input(total);
    Input(timeStepDays).imports("calendar[timeStepDays]");
    Input(doReset).equals(false);
    Input(isTicking).equals(true);
}

void PhysiologicalTime::update() {
    if (doReset) {
        step = total = 0.;
    }
    if (isTicking) {
        step = calcDailyTimeStep()*timeStepDays;
        total += step;
    }
}

} //namespace

