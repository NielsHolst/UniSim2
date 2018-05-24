/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
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
    char timeUnit;
    base::TimeUnit _timeUnit;

    // outputs
    QDate date;
    QTime time, trueSolarTime, sunrise, sunset;
    QDateTime dateTime;
    int dayOfYear, totalTime, totalTimeSteps;
    double timeStepSecs, timeStepDays, totalDays, dayLength,
        sinb, azimuth, solarConstant, angot;

    // methods
    void updateDerived();
    void updateSun();
    void updateAzimuth();
};

} //namespace
#endif
