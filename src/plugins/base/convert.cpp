/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <string>
#include <boost/spirit/home/x3.hpp>
#include <QVector>
#include "convert.h"
#include "value.h"

namespace x3 = boost::spirit::x3;
using boost::fusion::at_c;
using x3::int_;
using x3::ascii::space;

namespace base {

template<> QDate convert(QString x) {
    QDate result;
    auto const dmy_ymd = [&](auto& ctx) {
        int day, month, year;
        int i = at_c<0>(_attr(ctx)),
            j = at_c<1>(_attr(ctx)),
            k = at_c<2>(_attr(ctx));
        if (i<100) {
            day = i;
            month = j;
            year = k;
        }
        else {
            day = k;
            month = j;
            year = i;
        }
        result = QDate(year, month, day);
    };

    auto const mdy = [&](auto& ctx) {
        int month = at_c<0>(_attr(ctx)),
            day   = at_c<1>(_attr(ctx)),
            year  = at_c<2>(_attr(ctx));
        result = QDate(year, month, day);
    };

    std::string str = x.toStdString();
    auto first = str.begin(),
         last  = str.end();
    auto const date_def = (int_ >> '/' >> int_ >> '/' >> int_)  [dmy_ymd] |
                          (int_ >> '.' >> int_ >> '.' >> int_)  [dmy_ymd] |
                          (int_ >> '-' >> int_ >> '-' >> int_)  [dmy_ymd] |
                   ('/' >> int_ >> '/' >> int_ >> '/' >> int_)  [mdy];
    bool ok = x3::phrase_parse(
                first,
                last,
                date_def,
                space
                );
    if (!(first==last && ok && result.isValid()))
        ThrowException("Invalid date format").value(x);

    return result;
}

template<> QTime convert(QString x) {
    QStringList strings = x.split(":");
    QVector<int> numbers;
    bool ok;
    for (const QString &string : strings) {
        numbers << string.toInt(&ok);
        if (!ok) break;
    }
    if (!ok)
        ThrowException("Illegal time format").value(x);
    QTime result;
    switch (numbers.size()) {
    case 2:
        result = QTime(numbers.at(0), numbers.at(1));
        break;
    case 3:
        result = QTime(numbers.at(0), numbers.at(1), numbers.at(2));
        break;
    default:
        ok = false;
    }
    if (!(ok && result.isValid()))
        ThrowException("Illegal time format").value(x);
    return result;
}

template<> QDateTime convert(QString x) {
    QDateTime result;
    QStringList strings = x.split("T");
    if (strings.size() != 2)
        ThrowException("Illegal date-time format").value(x).
                hint("Put a 'T' beween the date and the time");
    result = QDateTime(convert<QDate>(strings.at(0)),
                       convert<QTime>(strings.at(1)), Qt::UTC);
    if (!result.isValid())
        ThrowException("Invalid date-time").value(x);
    return result;
}

template<> BareDate convert(QString x) {
    BareDate result;
    auto const dm = [&](auto& ctx) {
        int day = at_c<0>(_attr(ctx)),
            month = at_c<1>(_attr(ctx));
        result = BareDate(month, day);
    };

    auto const md = [&](auto& ctx) {
        int day = at_c<1>(_attr(ctx)),
            month = at_c<0>(_attr(ctx));
        result = BareDate(month, day);
    };

    std::string str = x.toStdString();
    auto first = str.begin(),
         last  = str.end();
    auto const bare_date_def = (int_ >> '/' >> int_) [dm] |
                        ('/' >> int_ >> '/' >> int_) [md];
    bool ok = x3::phrase_parse(
                first,
                last,
                bare_date_def,
                space
                );
    if (!(first==last && ok && result.date().isValid()))
        ThrowException("Invalid bare date format").value(x);

    return result;
}

//
// Vector value conversions
//

#define CONV_VALUE_VECTOR(vtype, type) \
template<> vtype convert(vValuePtr values) { \
    int n = values.size(); \
    vtype result(n); \
    const Value **source = values.data(); \
    type *dest           = result.data(); \
    for (int i=0; i<n; ++i) \
        *dest++ = (*source++)->as<type>(); \
    return result; \
}

CONV_VALUE_VECTOR(vbool, bool)
CONV_VALUE_VECTOR(vint, int)
CONV_VALUE_VECTOR(vdouble, double)
CONV_VALUE_VECTOR(vQString, QString)
CONV_VALUE_VECTOR(vQDate, QDate)
CONV_VALUE_VECTOR(vQTime, QTime)
CONV_VALUE_VECTOR(vQDateTime, QDateTime)
CONV_VALUE_VECTOR(vBareDate, BareDate)

//template<> vbool convert(vValuePtr values) {
//    int n = values.size();
//    vbool result(n);
//    const Value **source = values.data();
//    bool *dest           = result.data();
//    for (int i=0; i<n; ++i)
//        *dest++ = (*source++)->as<bool>();
//    return result;
//}


}
