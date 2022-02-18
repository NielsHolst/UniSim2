/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "exception.h"
#include "time_with_units.h"

namespace base {

QMap<TimeUnit, char> TimeWithUnits::_unitToChar;
QMap<char, TimeUnit> TimeWithUnits::_charToUnit;
QMap<TimeUnit, long long> TimeWithUnits::unitToSeconds;

TimeWithUnits::TimeWithUnits(int time, TimeUnit unit)
    : _time(time), _unit(unit)
{
}

char TimeWithUnits::unitToChar(TimeUnit unit) {
    if (_unitToChar.isEmpty()) {
        _unitToChar[Seconds] = 's';
        _unitToChar[Minutes] = 'm';
        _unitToChar[Hours] = 'h';
        _unitToChar[Days] = 'd';
        _unitToChar[Years] = 'y';
    }
    return _unitToChar.value(unit);
}

TimeUnit TimeWithUnits::charToUnit(char ch, Node *concerning) {
    if (ch < 'a')
        ch = 'a' + ch - 'A';
    if (_charToUnit.isEmpty()) {
        _charToUnit['s'] = Seconds;
        _charToUnit['m'] = Minutes;
        _charToUnit['h'] = Hours;
        _charToUnit['d'] = Days;
        _charToUnit['y'] = Years;
    }
    if (!_charToUnit.contains(ch))
        ThrowException("Unknown time unit").value(ch).context(concerning);
    return _charToUnit.value(ch);
}

double TimeWithUnits::conversionFactor(TimeUnit from, TimeUnit to) {
    return (from == to) ?  1. : TimeWithUnits(1,from).toSeconds()/double(TimeWithUnits(1,to).toSeconds());
}

long long TimeWithUnits::toSeconds() const {
    if (unitToSeconds.isEmpty()) {
        unitToSeconds[Seconds] = 1;
        unitToSeconds[Minutes] = 60;
        unitToSeconds[Hours] = 60*60;
        unitToSeconds[Days] = 24*60*60;
        unitToSeconds[Years] = 365*24*60*60;
    }
    return _time*unitToSeconds[_unit];
}


int TimeWithUnits::time() const {
    return _time;
}

TimeUnit TimeWithUnits::unit() const {
    return _unit;
}

QDateTime operator+(const QDateTime &dateTime, const TimeWithUnits &time) {
    int dt = time.time();
    switch (time.unit()) {
    case Seconds:
        return dateTime.addSecs(dt);
    case Minutes:
        return dateTime.addSecs(60*dt);
    case Hours:
        return dateTime.addSecs(60*60*dt);
    case Days:
        return dateTime.addDays(dt);
    case Years:
        QDate date = dateTime.date();
        QTime savedTime = dateTime.time();
        date = QDate(date.year() + dt, date.month(), date.day());
        return QDateTime(date, savedTime, dateTime.timeSpec());
    }
    Q_ASSERT(false);
    return QDateTime();
}

QDateTime operator+(const TimeWithUnits &time, const QDateTime &dateTime) {
    return dateTime + time;
}

}

