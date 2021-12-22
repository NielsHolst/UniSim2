/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CALENDAR_H
#define CALENDAR_H
#include <QDate>
#include <QDateTime>
#include <QTime>
#include <base/box.h>
#include <base/time_with_units.h>

namespace boxes {

class Calendar : public base::Box
{
public: 
    Calendar(QString name, QObject *parent);
	//standard methods
    void initialize();
    void reset();
	void update();

private:
    // inputs
    double latitude, longitude;
    QDateTime initialDateTime;
    int timeStep, timeZone, sample;
    QString timeUnit;
    base::TimeUnit _timeUnit;

    // outputs
    QDate date;
    QTime time;
    QDateTime dateTime;
    int dayOfYear, totalTime, totalTimeSteps;
    double timeStepSecs, timeStepDays, totalDays;

    // methods
    void updateDerived();
};

} //namespace
#endif
