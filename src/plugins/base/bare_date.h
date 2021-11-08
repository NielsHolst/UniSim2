/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_BARE_DATE_H
#define BASE_BARE_DATE_H

#include <QDate>

namespace base {

class BareDate : public QDate {
public:
    BareDate(int m, int d) : QDate(2001, m, d) {}
    BareDate() : QDate() {}
    // Wrap around at end of year
    BareDate addBareDays(int days) const {
        QDate date = addDays(days);
        return BareDate(date.month(), date.day());
    }
};

}

#endif

