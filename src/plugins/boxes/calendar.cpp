/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
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

constexpr double RAD = PI/180.;
constexpr double DEGREES = 1./RAD;

PUBLISH(Calendar)

Calendar::Calendar(QString name, QObject *parent)
    : Box(name, parent)
{
    help("keeps check on date, time and other sun-earth relations");
    Input(latitude).equals(52).help("Latitude").unit("[-90,90]");
    Input(longitude).equals(11).help("Longitiude").unit("[-180,180]");
    Input(timeZone).equals(1).help("Time zone").unit("h");
    Input(initialDateTime).equals(QDateTime(QDate(2000,1,1), QTime(0,0,0), Qt::UTC)).help("Date and time when calendar starts").unit("d/m/y h:m:s");
    Input(timeStep).equals(1).help("Time step in units of timeUnit").unit("int>0");
    Input(timeUnit).equals('d').help("Unit of time step").unit("y|d|h|m|s");
    Input(sample).equals(1).help("The frequency at which output is sampled").unit("int>0");
    Output(date).help("Current date").unit("d/m/y");
    Output(time).help("Current time of the day").unit("h:m:s");
    Output(trueSolarTime).help("Current true solar time of the day").unit("h:m:s");
    Output(dateTime).help("Current date and time").unit("d/m/y h:m:s");
    Output(timeStepSecs).help("Time step duration").unit("s");
    Output(timeStepDays).help("Time step duration").unit("d");
    Output(totalTimeSteps).help("Total number of time steps since calendar was reset").unit("int");
    Output(totalTime).help("Total time since calendar was reset; in units of timeUnit").unit("int");
    Output(totalDays).help("Total time since calendar was reset").unit("d");
    Output(dayOfYear).help("Julian day").unit("[1;366]");
    Output(dayLength).help("Astronomic day length").unit("h");;
    Output(sinb).help("Sine of solar height over the horizon").unit("[-1;1]");
    Output(azimuth).help("The compass direction of the sun relative to north").unit("[-180;180]");
    Output(sunrise).help("Time of sunrise").unit("h:m:s");
    Output(sunset).help("Time of sunset").unit("h:m:s");
    Output(solarConstant).help("The irradiation at the top of the atmosphere").unit("W/m2");
    Output(angot).help("The irradiation on Earth surface under optimal atmospheric conditions").unit("W/m2");
    Output(isDay).help("Tells of it's between sunrise and sunset").unit("bool");
    Output(isNight).help("Tells of it's between sunset and sunrise").unit("bool");
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
    _timeUnit = TimeWithUnits::charToUnit(timeUnit, this);
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
    updateSun();
    updateAzimuth();
    updateDayAndNight();
}

void Calendar::updateSun() {
/* See
 *  http://www.marsop.info/marsopdoc/metamp/05010401.HTM
 */
    double dec = -asin(sin(23.45*RAD)*cos(2*PI*(dayOfYear+10.)/365.)),
           sinLD = sin(RAD*latitude)*sin(dec),
           cosLD = cos(RAD*latitude)*cos(dec);
    Q_ASSERT(TestNum::neZero(cosLD));
    double aob = sinLD/cosLD;
    if (aob > 1) aob = 1.;
    if (aob < -1) aob = -1.;
    dayLength = 12.*(1. + 2.*asin(aob)/PI);
    int halfDay = dayLength/2.*60*60;
    sunrise = QTime(12,00).addSecs(-halfDay); // Shouldn't solar noon be used?
    sunset = QTime(12,00).addSecs(halfDay);

    double h = time.hour() + time.minute()/60. + time.second()/3600.;
    sinb = sinLD + cosLD*cos(2.*PI*(h + 12.)/24.);
    if (sinb < 0.) sinb = 0.;
    // From Kropff & Laar (1993), pp. 235-236
    double dsinb = 3600.*(dayLength*sinLD + 24.*cosLD*sqrt(1. - aob*aob)/PI);
    double dsinbe = 3600.*(dayLength*(sinLD + 0.4*(sinLD*sinLD + cosLD*cosLD*0.5)) +
                           12.*cosLD*(2. + 3.*0.4*sinLD)*sqrt(1. - aob*aob)/PI);
    solarConstant = 1370.*(1. + 0.033*cos(2.*PI*dayOfYear/365.));
    angot = solarConstant*dsinb*sinb*(1. + 0.4*sinb)/dsinbe; // This is symmetrical around noon. So, shouldn't solar time be used?
}

//! Azimuth is 90 at noon, zero at sunset and sunrise, and -90 at midnight
/* See
 *  http://www.esrl.noaa.gov/gmd/grad/solcalc/calcdetails.html
 *  http://www.jgiesen.de/astro/suncalc/calculations.htm
*/
void Calendar::updateAzimuth() {
    // First, the fractional year y is calculated, in radians.
    double y = 2*PI*(dayOfYear-1+(time.hour()-12.)/24)/365.;

    //From y, we can estimate the equation of time (in minutes) and the solar declination angle (in radians).
    double eqtime =
    229.18*( 0.000075+0.001868*cos(y)-0.032077*sin(y)-0.014615*cos(2*y)-0.040849*sin(2*y) );

    double declin = 0.006918-0.399912*cos(y)+0.070257*sin(y)
            -0.006758*cos(2*y)+0.000907*sin(2*y)
            -0.002697*cos(3*y)+0.00148*sin(3*y);

    // Next, the true solar time is calculated in the following two equations. First the time offset is
    // found, in minutes, and then the true solar time, in minutes.
    double timeOffset = eqtime - 4*longitude + 60*timeZone; // minutes
    // where eqtime is in minutes, longitude is in degrees, timezone is in hours from UTC

    // True solar time
    double tst = time.hour()*60 + time.minute() + time.second()/60. + timeOffset; // minutes
    trueSolarTime = QTime(0,0,0).addSecs(int(tst+0.5)*60);
//    trueSolarTime = QTime::fromMSecsSinceStartOfDay(int(tst+0.5)*60*1000);
    if (!trueSolarTime.isValid())
        trueSolarTime = QTime(0,0);

    // The solar hour angle, in radians, is:
    double ha = (tst/4 - 180)*RAD;

    // The solar zenith angle (phi) can then be found from the following equation:
    double lat = latitude*RAD;
    double cosPhi = sin(lat)*sin(declin) + cos(lat)*cos(declin)*cos(ha);
    double phi = acos(cosPhi);

    // And the solar azimuth is:
    double az1 = -(sin(lat)*cos(phi)-sin(declin))/(cos(lat)*sin(phi));
    double az2 = acos(az1)*DEGREES;
    azimuth = az2-90.;
}

void Calendar::updateDayAndNight() {
    isNight = (time > sunset) || (time < sunrise);
    isDay = !isNight;
}

} //namespace

