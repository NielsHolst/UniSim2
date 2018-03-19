/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
    help("help defining a time interval for the simulation");
    Input(beginDate).help("Simulation start date");
    Input(endDate).help("Simulation end date");
    Input(beginTime).equals(QTime(0,0,0)).help("Simulation start time");
    Input(endTime).equals(QTime(0,0,0)).help("Simulation end time");
    Input(timeStep).equals(5).help("Time step length");
    Input(timeUnit).equals('m').help("Time step unit");
    Output(steps).help("Number of steps in simulation");
    Output(beginDateTime).help("Date and time to begin simulation");
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

