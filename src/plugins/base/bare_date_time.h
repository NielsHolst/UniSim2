/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_BARE_DATE_TIME_H
#define BASE_BARE_DATE_TIME_H
#include <QDate>
#include <QDateTime>
#include <QTime>
#include "bare_date.h"

namespace base {

class BareDateTime : public QDateTime {
public:
    BareDateTime(BareDate date, QTime time) : QDateTime(QDate(2001, date.month(), date.day()), time, Qt::UTC) {}
    BareDateTime() : QDateTime() {}
    // Wrap around at end of year
    BareDateTime addBareDays(int days) const {
        QDate datePart = date();
        QTime timePart = time();
        datePart = datePart.addDays(days);
        BareDate bareDate = BareDate(datePart.month(), datePart.day());
        return BareDateTime(bareDate, timePart);
    }
};

}

#endif

