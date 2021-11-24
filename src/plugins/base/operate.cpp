/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <boost/numeric/conversion/cast.hpp>
#include <QDate>
#include <QDateTime>
#include <QObject>
#include <QString>
#include <QTime>
#include "bare_date.h"
#include "convert.h"
#include "exception.h"
#include "operate.h"

using namespace base;

namespace operate {

Value add(const Value &a, const Value &b) {
    double u, v;
    using Type = Value::Type;

    switch (a.type()) {
    case Type::Uninitialized :
        break;
    case Type::Bool:
        break;
    case Type::Int:
        switch (b.type()) {
        case Type::Int     : return Value(a.value<int>() + b.value<int>());
        case Type::Double  : return Value(a.value<double>() + b.value<double>());
        case Type::Date    : return Value(b.value<QDate>().addDays(a.value<int>()));
        case Type::Time    : return Value(b.value<QTime>().addMSecs(3600*1000*a.value<int>()));
        case Type::DateTime: return Value(b.value<QDateTime>().addDays(a.value<int>()));
        case Type::BareDate: return Value(b.value<BareDate>().addBareDays(a.value<int>()));
        default            : ;
        }
        break;
    case Type::Double:
        switch (b.type()) {
        case Type::Uninitialized : break;
        case Type::Int     :
        case Type::Double  : return Value(a.value<double>() + b.value<double>());
        case Type::Date    : return Value(b.value<QDate>().addDays(a.value<int>()));
        case Type::Time    : return Value(b.value<QTime>().addMSecs(3600*1000*a.value<int>()));
        case Type::DateTime: return Value(b.value<QDateTime>().addDays(a.value<int>()));
        case Type::BareDate: return Value(b.value<BareDate>().addBareDays(a.value<int>()));
        default            : ;
        }
        break;
    case Type::String:
        switch (b.type()) {
        case Type::String  : return Value(a.value<QString>() + b.value<QString>());
        default            : ;
        }
        break;
    case Type::Date:
        switch (b.type()) {
        case Type::Int     :
        case Type::Double  : return Value(a.value<QDate>().addDays(b.value<int>()));
        default            : ;
        }
        break;
    case Type::Time:
        switch (b.type()) {
        case Type::Int     :
        case Type::Double  : return Value(a.value<QTime>()
                                           .addMSecs(convert<int>(3600*1000*b.value<double>()))
                                         );
        default            : ;
        }
        break;
    case Type::DateTime:
        switch (b.type()) {
        case Type::Int     : return Value(a.value<QDateTime>().addDays(b.value<int>()));
        case Type::Double  : u = b.value<double>();
                             v = floor(u);
                             return Value(a.value<QDateTime>()
                                           .addDays(convert<int>(v))
                                           .addSecs(convert<int>(24*3600*(u-v)))
                                         );
        default            : ;
        }
        break;
    case Type::BareDate:
        switch (b.type()) {
        case Type::Int     :
        case Type::Double  : return Value(a.value<BareDate>().addBareDays(b.value<int>()));
        default            : ;
        }
        break;
    case Type::VecBool:
        break;
    case Type::VecInt:
        break;
    case Type::VecDouble:
        break;
    }
    ThrowException("Incompatible operands").value1(a.typeName()).value2(b.typeName()); \
    return Value();
}


}

