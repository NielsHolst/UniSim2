/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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

