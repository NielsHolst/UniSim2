/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/time_with_units.h>
#include <base/publish.h>
#include "simulation_period.h"

using namespace base;

namespace vg {

PUBLISH(SimulationPeriod)

SimulationPeriod::SimulationPeriod(QString name, QObject *parent)
    : Box(name, parent)
{
    help("help defining a time interval for the simulation");
    Input(weatherFirstDateTime).imports("outdoors/records[firstDateTime]").unit("DateTime");
    Input(beginDate).help("Simulation start date").unit("Date");
    Input(endDate).help("Simulation end date").unit("Date");
    Input(beginTime).equals(QTime(0,0,0)).help("Simulation start time").unit("Time");
    Input(endTime).equals(QTime(0,0,0)).help("Simulation end time").unit("Time");
    Input(timeStep).equals(2).help("Time step length").unit("-");
    Input(timeUnit).equals('m').help("Time step unit").unit("y|d|h|m|s");
    Output(steps).help("Number of steps in simulation").unit("-");
    Output(beginDateTime).help("Date and time to begin simulation").unit("DateTime");
}

void SimulationPeriod::reset() {
    // Begin one day before to warm up model
    beginDateTime = QDateTime(alignYear(beginDate), beginTime, Qt::UTC).addDays(-1);
    steps = secsInterval() / secsTimeStep() + 1;
    if (steps < 1) {
        ThrowException("Wrong time interval: beginDate must be before endDate").
                value(beginDate).value2(endDate).context(this);
    }
    // If end is at midnight then assume that it's at the end of the day
    if (endTime == QTime(0,0,0))
        endDate = endDate.addDays(1);
}

QDate SimulationPeriod::alignYear(QDate date) {
    // Align year with year of weather records
    int weatherYear = weatherFirstDateTime.date().year();
    return QDate(weatherYear, date.month(), date.day());
}

int SimulationPeriod::secsInterval() {
    QDateTime end(alignYear(endDate), endTime);
    return static_cast<int>(beginDateTime.secsTo(end)) + 3600;
}

int SimulationPeriod::secsTimeStep() {
    TimeUnit timeUnit_ = TimeWithUnits::charToUnit(timeUnit, this);
    return static_cast<int>(timeStep*TimeWithUnits::conversionFactor(timeUnit_, Seconds));
}

} //namespace

