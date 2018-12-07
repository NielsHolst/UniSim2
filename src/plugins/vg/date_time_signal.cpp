/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "date_time_signal.h"

using namespace base;

namespace vg {

PUBLISH(DateTimeSignal)

DateTimeSignal::DateTimeSignal(QString name, QObject *parent)
    : BaseSignal(name, parent){
    help("Sets a signal according to date and time of day");
    Input(beginDate).equals(QDate(2001,1,1)).help("The beginning of the date interval [1..366]");
    Input(endDate).equals(QDate(2001,12,31)).help("The end of the date interval [1..366]");
    Input(beginTime).equals(QTime(0,0)).help("The beginning of the time interval [hh:mm]");
    Input(endTime).equals(QTime(23,59,59)).help("The end of the time interval [hh:mm]");
    Input(day).imports("calendar[dayOfYear]");
    Input(time).imports("calendar[time]");
    Input(signalInside).equals(1.).help("Resulting signal when day and time are inside the day and time intervals");
    Input(signalOutside).equals(0.).help("Resulting signal when day and time are outside the day and time intervals");
    Input(signalOutsideTimeOnly).equals(0.).help("Resulting signal when day is inside its interval but time is outside its interval");
    Input(circadian).equals(true).help("Tells whether signal follows a daily rythm");
}

inline QDate date(int day) {
    return QDate(2000,12,31).addDays(day);
}

inline QDate date(QDate date) {
    return QDate(2001, date.month(), date.day());
}

double DateTimeSignal::computeSignal() {
    QDate today = date(day);
    beginDate = date(beginDate);
    endDate = date(endDate);
    bool nowOnDay = (beginDate < endDate) ?
                (today >= beginDate) && (today <= endDate) :
                (today >= beginDate) || (today <= endDate);
    bool nowOnTime = (beginTime < endTime) ?
                (time >= beginTime) && (time <= endTime) :
                (time >= beginTime) || (time <= endTime);
    double result;
    if (circadian) {
        result =  nowOnDay ?
                  (nowOnTime ? signalInside : (signalOutsideTimeOnly))
                  : signalOutside;
    }
    else {
        QDateTime begin(beginDate, beginTime),
                  end(endDate, endTime),
                  now(today, time);
        result = (now >= begin && now <= end) ?
                 signalInside : signalOutside;
    }
    return result;
}

} //namespace

