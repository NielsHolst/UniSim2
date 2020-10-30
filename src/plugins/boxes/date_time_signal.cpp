/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "date_time_signal.h"

using namespace base;

namespace boxes {

PUBLISH(DateTimeSignal)

DateTimeSignal::DateTimeSignal(QString name, QObject *parent)
    : BaseSignal(name, parent){
    help("sets a signal according to date (ignoring year) and time of day");
    Input(beginDate).equals(QDate(2001,1,1)).help("The beginning of the date interval").unit("d/m/y");
    Input(endDate).equals(QDate(2001,12,31)).help("The end of the date interval [1..366]").unit("d/m/y");
    Input(beginTime).equals(QTime(0,0)).help("The beginning of the time interval [hh:mm]").unit("h:m:s");
    Input(endTime).equals(QTime(23,59,59)).help("The end of the time interval [hh:mm]").unit("h:m:s");
    Input(day).imports("calendar[dayOfYear]").unit("[1:366]");
    Input(time).imports("calendar[time]").unit("h:m:s");
    Input(signalInside).equals(1.).help("Resulting signal when day and time are inside the day and time intervals");
    Input(signalOutside).equals(0.).help("Resulting signal when day and time are outside the day and time intervals");
    Input(circadian).equals(true).help("Tells whether signal follows a daily rythm").unit("y|n");
}

inline QDate date(int day) {
    return QDate(2000,12,31).addDays(day);
}

inline QDate date(QDate date) {
    return QDate(2001, date.month(), date.day());
}

bool DateTimeSignal::computeFlag() {
    QDate today = date(day);
    beginDate = date(beginDate);
    endDate = date(endDate);
    bool nowOnDay = (beginDate < endDate) ?
                (today >= beginDate) && (today <= endDate) :
                (today >= beginDate) || (today <= endDate);
    bool nowOnTime = (beginTime < endTime) ?
                (time >= beginTime) && (time <= endTime) :
                (time >= beginTime) || (time <= endTime);
    bool flag;
    if (circadian) {
        flag =  nowOnDay && nowOnTime;
    }
    else {
        QDateTime begin(beginDate, beginTime),
                  end(endDate, endTime),
                  now(today, time);
        flag = (now >= begin && now <= end);
    }
    return flag;
}

double DateTimeSignal::computeSignal(bool flag) {
    return flag ? signalInside : signalOutside;
}

} //namespace

