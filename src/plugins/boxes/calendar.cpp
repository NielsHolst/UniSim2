/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
/* Day length and solar elevation equations copied from the FORTRAN code of Kroppf &
** Laar (1993). Modelling Crop-Weed Interactions. CAB International.
*/
#include <boost/math/constants/constants.hpp>
#include <QTextStream>
#include <cfloat>
#include <cmath>
#include <base/publish.h>
#include <base/test_num.h>
#include "calendar.h"

using namespace std;
using namespace base;

namespace boxes {

constexpr double PI = boost::math::constants::pi<double>();
constexpr double RAD = PI/180;
constexpr double DEGREES = 1./RAD;

PUBLISH(Calendar)

/*! \class Calendar
 * \brief Calendar is ...
 *
 * Inputs
 * ------
 * - _latitude_ is geographical latitude of the system [-180;180]
 * - _longitude_ is geographical longitude of the system [-180;180]
 * - _timeZone_ is time zone in hours relative to GMT, e.g., Copenhagen has_timeZone_ = 1 [-12..12]
 * - _initialDate_ is the initial date of the simulation; default is 1/1/2000 [DD/MM/YYYY or YYYY/MM/DD]
 * - _initialTimeOfDay_ is the initial time of the simulation;
 * default is midnight, i.e. at the beginning of _initialDate_ [HH/MM/SS or HH/MM]
 * - _timeStep_ is the duration of one integration time step in units determined by _timeUnit_ [1..N]
 * - _timeUnit_ is the unit of _timeStep_ [s,m,h,d,y]
 * - _sample_ is the default sampling frequency for output; default is 1 [1..N]. Example: If _timeStep_ = 5 and _timeUnit_ = m
 * then _sample_ = 12 will yield an output every hour
 *
 * Outputs
 * -------
 * - _date_ is the current date [QDate]
 * - _timeOfDay_ is the current time of day [QTime]
 * - _trueSolarTime_ is the true solar time [QTime]
 * - _dateTime_ is the current date and time [QDateTime]
 * - _timeStepSecs_ is the duration of _timeStep_ in seconds [s]
 * - _totalTimeSteps_ is the total number of time steps performed since beginning (reset) of simulation [0..N]
 * - _totalTime_ is like _totalTimeSteps_ but in units determined by _timeUnit_ [0..N]
 * - _totalDays_ is like _totalTimeSteps_ but in days [R]
 * - _dayOfYear_ is the day number in year, also known as Julian day
 * - _day_ is the current day of the month [1..31]
 * - _month_ is the current month of the year [1..12]
 * - _year_ is the current year [N]
 * - _hour_ is the current hour of the day [0..23]
 * - _minute_ is the current minute of the hour [0..59)]
 * - _second_ is the current second of the minute [0..59]
 * - _dateAsReal_ is _date:_ as a real number measured in years [R]
 * - _dayLength_ is day length [h]
 * - _sinb_ is the sine of sun elevation [-1;1]
 * - _azimuth_ is the azimuth of the sun relative to north [-180;180]
 * - _sunrise_ is the time (needs adjustment to local/astronomic time!) of sunrise [QTime]
 * - _sunset_ is the time (needs adjustment to local/astronomic time!) of sunset [QTime]
 * - _solarConstant_ is the solar constant, i.e. irradiation at the top of the atmsphere [MJ/m<SUP>2</SUP>/d]
 * - _angot_ is the Angot value, i.e. the irradiation reaching the Earth surface
 * under optimal atmospheric conditions [MJ/m<SUP>2</SUP>/d]
 * - _irradiationCorrection_ is a correction factor on the daily irradiation [MJ/m<SUP>2</SUP>/d]
 * depending on sun elevation [R]
 */

Calendar::Calendar(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(Calendar);
    Input(latitude).equals(52);
    Input(longitude).equals(11);
    Input(timeZone).equals(1);
    Input(initialDate).equals(QDate(2000,1,1));
    Input(initialTimeOfDay).equals(QTime(0,0,0));
    Input(timeStep).equals(1);
    Input(timeUnit).equals('d');
    Input(sample).equals(1);
    Output(date);
    Output(timeOfDay);
    Output(trueSolarTime);
    Output(dateTime);
    Output(timeStepSecs);
    Output(totalTimeSteps);
    Output(totalTime);
    Output(totalDays);
    Output(dayOfYear);
    Output(day);
    Output(month);
    Output(year);
    Output(hour);
    Output(minute);
    Output(second);
    Output(dateAsReal);
    Output(dayLength);
    Output(sinb);
    Output(azimuth);
    Output(sunrise);
    Output(sunset);
    Output(solarConstant);
    Output(angot);
    Output(irradiationCorrection);
}

void Calendar::initialize() {
    if (sample < 1) sample = 1; // To increase user robustness
}

void Calendar::reset() {
    _timeUnit = TimeWithUnits::charToUnit(timeUnit, this);
    timeStepSecs = timeStep*TimeWithUnits::conversionFactor(_timeUnit, Seconds);
    dateTime = QDateTime(initialDate, initialTimeOfDay, Qt::UTC);
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
    timeOfDay = dateTime.time();
    day = date.day();
    month = date.month();
    dayOfYear = date.dayOfYear();
    year = date.year();
    hour = timeOfDay.hour();
    minute = timeOfDay.minute();
    second = timeOfDay.second();

    totalTime = totalTimeSteps*timeStep;
    totalDays = totalTime*TimeWithUnits::conversionFactor(_timeUnit, Days);

    QDateTime beginning = QDateTime(QDate(year,1,1), QTime(), Qt::UTC);
    double secsPassed = beginning.secsTo(dateTime);
    double secsInYear = date.daysInYear()*24*60*60;
    dateAsReal = double(year) + secsPassed/secsInYear;

    updateSun();
    updateRadiation();
    updateAzimuth();
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
    dayLength = 12.*(1. + 2.*asin(aob)/PI);
    int halfDay = dayLength/2.*60*60;
    sunrise = QTime(12,00).addSecs(-halfDay);
    sunset = QTime(12,00).addSecs(halfDay);

    double h = hour + minute/60. + second/3600.;
    sinb = sinLD + cosLD*cos(2.*PI*(h + 12.)/24.);
    if (sinb < 0.) sinb = 0.;

    double dsinb = 3600.*(dayLength*sinLD + 24.*cosLD*sqrt(1. - aob*aob)/PI);
    double dsinbe = 3600.*(dayLength*(sinLD + 0.4*(sinLD*sinLD + cosLD*cosLD*0.5)) +
                           12.*cosLD*(2. + 3.*0.4*sinLD)*sqrt(1. - aob*aob)/PI);
    solarConstant = 1370.*(1. + 0.033*cos(2.*PI*day/365.));
    angot = solarConstant*dsinb*1e-6;
    irradiationCorrection = sinb*(1. + 0.4*sinb)/dsinbe;
}

void Calendar::updateRadiation() {

}

//! Azimuth is 90 at noon, zero at sunset and sunrise, and -90 at midnight
/* See
 *  http://www.esrl.noaa.gov/gmd/grad/solcalc/calcdetails.html
 *  http://www.jgiesen.de/astro/suncalc/calculations.htm
*/
void Calendar::updateAzimuth() {
    // First, the fractional year y is calculated, in radians.
    double y = 2*PI*(dayOfYear-1+(hour-12.)/24)/365.;

    //From y, we can estimate the equation of time (in minutes) and the solar declination angle (in radians).
    double eqtime =
    229.18*( 0.000075+0.001868*cos(y)-0.032077*sin(y)-0.014615*cos(2*y)-0.040849*sin(2*y) );

    double declin = 0.006918-0.399912*cos(y)+0.070257*sin(y)
            -0.006758*cos(2*y)+0.000907*sin(2*y)
            -0.002697*cos(3*y)+0.00148*sin(3*y);

    // Next, the true solar time is calculated in the following two equations. First the time offset is
    // found, in minutes, and then the true solar time, in minutes.
    double timeOffset = eqtime - 4*longitude + 60*timeZone;
    // where eqtime is in minutes, longitude is in degrees, timezone is in hours from UTC

    // True solar time
    double tst = hour*60 + minute + second/60. + timeOffset;
    trueSolarTime = QTime::fromMSecsSinceStartOfDay(int(tst+0.5)*60*1000);
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

} //namespace

