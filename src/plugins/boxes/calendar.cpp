/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
/* Day length and solar elevation equations copied from the FORTRAN code of Kroppf &
** Laar (1993). Modelling Crop-Weed Interactions. CAB International.
*/
#include <cfloat>
#include <cmath>
#include <QTextStream>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "calendar.h"

using namespace std;
using namespace base;
using namespace phys_math;

namespace boxes {

PUBLISH(Calendar)

Calendar::Calendar(QString name, QObject *parent)
    : Box(name, parent)
{
    help("keeps check on date and time");
    Input(latitude).equals(52).help("Latitude").unit("[-90,90]");
    Input(longitude).equals(11).help("Longitiude").unit("[-180,180]");
    Input(timeZone).equals(1).help("Time zone").unit("h");
    Input(initialDateTime).equals(QDateTime(QDate(2000,1,1), QTime(0,0,0), Qt::UTC)).help("Date and time when calendar starts").unit("d/m/y h:m:s");
    Input(timeStep).equals(1).help("Time step in units of timeUnit").unit("int>0");
    Input(timeUnit).equals("d").help("Unit of time step").unit("y|d|h|m|s");
    Input(sample).equals(1).help("The frequency at which output is sampled").unit("int>0");

    Output(date).help("Current date").unit("d/m/y");
    Output(time).help("Current time of the day").unit("h:m:s");
    Output(dateTime).help("Current date and time").unit("d/m/y h:m:s");
    Output(timeStepSecs).help("Time step duration").unit("s");
    Output(timeStepDays).help("Time step duration").unit("d");
    Output(totalTimeSteps).help("Total number of time steps since calendar was reset").unit("int");
    Output(totalTime).help("Total time since calendar was reset; in units of timeUnit").unit("int");
    Output(totalDays).help("Total time since calendar was reset").unit("d");
    Output(dayOfYear).help("Julian day").unit("[1;366]");
}

void Calendar::initialize() {
    // Some classes may need the outputs already in their initialize step
    reset();
}

void Calendar::reset() {
    if (sample < 1)
        ThrowException("Sample frequency ('sample') must be larger than zero").value(sample);
    if (timeStep < 1)
        ThrowException("Time step ('timeStep') must be larger than zero").value(timeStep);
    _timeUnit = TimeWithUnits::charToUnit(timeUnit.at(0).toLatin1(), this);
    timeStepSecs = timeStep*TimeWithUnits::conversionFactor(_timeUnit, Seconds);
    timeStepDays = timeStep*TimeWithUnits::conversionFactor(_timeUnit, Days);
    dateTime = initialDateTime;
    dateTime.setTimeSpec(Qt::UTC);
    totalTimeSteps = 0;
    updateDerived();
}

void Calendar::update() {
    ++totalTimeSteps;
    dateTime = dateTime + TimeWithUnits(timeStep, _timeUnit);
    updateDerived();
}

void Calendar::updateDerived() {
    date = dateTime.date();
    time = dateTime.time();
    dayOfYear = date.dayOfYear();
    totalTime = totalTimeSteps*timeStep;
    totalDays = totalTime*TimeWithUnits::conversionFactor(_timeUnit, Days);
}

} //namespace

