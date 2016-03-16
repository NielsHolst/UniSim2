/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/time_with_units.h>
#include <base/publish.h>
#include "simulation_time_interval.h"

using namespace base;

namespace vg {

PUBLISH(SimulationTimeInterval)

SimulationTimeInterval::SimulationTimeInterval(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(beginDate);
    Input(endDate);
    Input(beginTime);
    Input(endTime);
    Input(timeStep).equals(1);
    Input(timeUnit).equals('d');
    Output(steps);
    Output(beginDateTime);
}

void SimulationTimeInterval::reset() {
    beginDateTime = QDateTime(beginDate, beginTime, Qt::UTC);
    steps = secsInterval() / secsTimeStep();
    if (steps < 1) steps = 1;
}

long long int SimulationTimeInterval::secsInterval() {
    QDateTime begin(beginDate, beginTime);
    QDateTime end(endDate, endTime);
    return begin.secsTo(end);
}

long long int SimulationTimeInterval::secsTimeStep() {
    TimeUnit timeUnit_ = TimeWithUnits::charToUnit(timeUnit, this);
    return timeStep*TimeWithUnits::conversionFactor(timeUnit_, Seconds);
}

} //namespace

