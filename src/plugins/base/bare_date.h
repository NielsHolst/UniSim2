/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_BARE_DATE_H
#define BASE_BARE_DATE_H
#include <QDate>

namespace base {

class BareDate {
public:
    BareDate(int m, int d) : _date(2001, m, d) {}
    BareDate() : _date() {}
    BareDate addBareDays(int days) const {
        // Wrap around at end of year
        QDate newDate = _date.addDays(days);
        return BareDate(newDate.month(), newDate.day());
    }
    QDate date() const { return _date; }

private:
    QDate _date;
};

// Comparisons

inline bool  operator==(const BareDate &a, const BareDate &b) {
    return (a.date() == b.date());
}

inline bool  operator!=(const BareDate &a, const BareDate &b) {
    return (a.date() != b.date());
}

inline bool operator<(const BareDate &a, const BareDate &b) {
    return a.date() < b.date();
}

inline bool operator<=(const BareDate &a, const BareDate &b) {
    return a.date() <= b.date();
}

inline bool operator>(const BareDate &a, const BareDate &b) {
    return a.date() > b.date();
}

inline bool operator>=(const BareDate &a, const BareDate &b) {
    return a.date() >= b.date();
}

// Addition

BareDate operator+(const BareDate &bare, double x);
BareDate operator+(double x, const BareDate &bare);

// Subtraction

BareDate operator-(const BareDate &bare, double x);
int operator-(const BareDate &a, const BareDate &b);

}

#endif

