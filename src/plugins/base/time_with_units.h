/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_TIME_WITH_UNITS_H
#define BASE_TIME_WITH_UNITS_H
#include <QDateTime>
#include <QMap>
#include <QString>

class QObject;

namespace base {

enum TimeUnit{Seconds, Minutes, Hours, Days, Years};

class TimeWithUnits {
public:
    TimeWithUnits(int time, TimeUnit unit);
    int time() const;
    TimeUnit unit() const;
    long long toSeconds() const;
    static char unitToChar(TimeUnit unit);
    static TimeUnit charToUnit(char ch, QObject *concerning = 0);
    static double conversionFactor(TimeUnit from, TimeUnit to);
private:
    int _time;
    TimeUnit _unit;
    static QMap<TimeUnit, char> _unitToChar;
    static QMap<char, TimeUnit> _charToUnit;
    static QMap<TimeUnit, long long> unitToSeconds;
};

QDateTime operator+(const QDateTime &dateTime, const TimeWithUnits &time);
QDateTime operator+(const TimeWithUnits &time, const QDateTime &dateTime);

} //namespace

#endif
