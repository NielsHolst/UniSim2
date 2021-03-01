/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "any_year.h"

namespace base {

const int ANY_YEAR = 1000000000;

int anyYear() {
    return ANY_YEAR;
}

void setAnyYear(QDate &date) {
    date = QDate(ANY_YEAR, date.month(), date.day());
}

inline void alignYear(QDate &date1, QDate &date2) {
    if (hasAnyYear(date1) || hasAnyYear(date2)) {
        setAnyYear(date1);
        setAnyYear(date2);
    }
}

bool hasAnyYear(QDate date) {
    return date.year() == ANY_YEAR;
}

bool equals(QDate date1, QDate date2) {
    alignYear(date1, date2);
    return date1 == date2;
}

bool isBefore(QDate date1, QDate date2) {
    alignYear(date1, date2);
    return date1 < date2;
}

bool isAfter(QDate date1, QDate date2) {
    alignYear(date1, date2);
    return date1 > date2;
}

bool isBeforeOrEquals(QDate date1, QDate date2) {
    alignYear(date1, date2);
    return date1 <= date2;
}

bool isAfterOrEquals(QDate date1, QDate date2) {
    alignYear(date1, date2);
    return date1 >= date2;
}



}

