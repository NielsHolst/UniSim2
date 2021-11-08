/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <boost/numeric/conversion/cast.hpp>
#include <boost/variant/get.hpp>
#include "exception.h"
#include "expression_operation.h"

using boost::get;
using boost::numeric_cast;
using namespace base;

namespace expression {

inline int daysToDays(double x) {
    return numeric_cast<int>(x+0.5);
}

inline int daysToSecs(double x) {
    return numeric_cast<int>(x*24*3600+0.5);
}

inline int hoursToSecs(int x) {
    return x*3600;
}

inline int hoursToSecs(double x) {
    return numeric_cast<int>(x*3600+0.5);
}

Operand add(const Operand &a, const Operand &b) {
    switch(a.type()) {
    case Operand::Type::Bool:
    case Operand::Type::BoolPtr:
        break;
    case Operand::Type::Char:
        switch(b.type()) {
        case Operand::Type::String:       return Operand{QString(get<char>(a)) + get<QString>(b)};
        default: ;
        }
        break;
    case Operand::Type::CharPtr:
        switch(b.type()) {
        case Operand::Type::String:       return Operand{QString(*get<char*>(a)) + get<QString>(b)};
        default: ;
        }
        break;
    case Operand::Type::Int:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{get<int>(a) + get<int>(b)};
        case Operand::Type::Double:       return Operand{get<int>(a) + get<double>(b)};
        case Operand::Type::Date:         return Operand{get<QDate    >(b).addDays(get<int>(a))};
        case Operand::Type::DateTime:     return Operand{get<QDateTime>(b).addDays(get<int>(a))};
        case Operand::Type::Time:         return Operand{get<QTime>(b).addSecs(3600*get<int>(a))};
        case Operand::Type::BareDate:     return Operand{get<BareDate    >(b).addBareDays(get<int>(a))};
        case Operand::Type::BareDateTime: return Operand{get<BareDateTime>(b).addBareDays(get<int>(a))};
        case Operand::Type::String:       return Operand{QString::number(get<int>(a)) + get<QString>(b)};
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{get<int>(a) + *get<int*>(b)};
        case Operand::Type::DoublePtr:    return Operand{get<int>(a) + *get<double*>(b)};
        case Operand::Type::DatePtr:      return Operand{(*get<QDate*    >(b)).addDays(get<int>(a))};
        case Operand::Type::DateTimePtr:  return Operand{(*get<QDateTime*>(b)).addDays(get<int>(a))};
        case Operand::Type::TimePtr:      return Operand{(*get<QTime*>(b)).addSecs(3600*get<int>(a))};
        case Operand::Type::BareDatePtr:  return Operand{(*get<BareDate*    >(b)).addDays(get<int>(a))};
        case Operand::Type::BareDateTimePtr: return Operand{(*get<BareDateTime*>(b)).addDays(get<int>(a))};
        case Operand::Type::StringPtr:    return Operand{QString::number(get<int>(a)) + *get<QString*>(b)};
        }
        break;
    case Operand::Type::IntPtr:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{*get<int*>(a) + get<int>(b)};
        case Operand::Type::Double:       return Operand{*get<int*>(a) + get<double>(b)};
        case Operand::Type::Date:         return Operand{get<QDate    >(b).addDays(*get<int*>(a))};
        case Operand::Type::DateTime:     return Operand{get<QDateTime>(b).addDays(*get<int*>(a))};
        case Operand::Type::Time:         return Operand{get<QTime>(b).addSecs(*get<int*>(a)*3600)};
        case Operand::Type::BareDate:     return Operand{get<BareDate    >(b).addDays(*get<int*>(a))};
        case Operand::Type::BareDateTime: return Operand{get<BareDateTime>(b).addDays(*get<int*>(a))};
        case Operand::Type::String:       return Operand{QString::number(*get<int*>(a)) + get<QString>(b)};
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{*get<int*>(a) + *get<int*>(b)};
        case Operand::Type::DoublePtr:    return Operand{*get<int*>(a) + *get<double*>(b)};
        case Operand::Type::DatePtr:      return Operand{(*get<QDate*    >(b)).addDays(*get<int*>(a))};
        case Operand::Type::DateTimePtr:  return Operand{(*get<QDateTime*>(b)).addDays(*get<int*>(a))};
        case Operand::Type::TimePtr:      return Operand{(*get<QTime*>(b)).addSecs(*get<int*>(a)*3600)};
        case Operand::Type::BareDatePtr:  return Operand{(*get<BareDate*    >(b)).addDays(*get<int*>(a))};
        case Operand::Type::BareDateTimePtr: return Operand{(*get<BareDateTime*>(b)).addDays(*get<int*>(a))};
        case Operand::Type::StringPtr:    return Operand{QString::number(*get<int*>(a)) + *get<QString*>(b)};
        }
        break;
    case Operand::Type::Double:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{get<double>(a) + get<int>(b)};
        case Operand::Type::Double:       return Operand{get<double>(a) + get<double>(b)};
        case Operand::Type::Date:         return Operand{get<QDate    >(b).addDays(daysToDays(get<double>(a)))};
        case Operand::Type::DateTime:     return Operand{get<QDateTime>(b).addSecs(daysToSecs(get<double>(a)))};
        case Operand::Type::Time:         return Operand{get<QTime>(b).addSecs(hoursToSecs(get<double>(a)))};
        case Operand::Type::BareDate:     return Operand{get<BareDate    >(b).addDays(daysToDays(get<double>(a)))};
        case Operand::Type::BareDateTime: return Operand{get<BareDateTime>(b).addDays(daysToDays(get<double>(a)))};
        case Operand::Type::String:       return Operand{QString::number(get<double>(a)) + get<QString>(b)};
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{get<double>(a) + *get<int*>(b)};
        case Operand::Type::DoublePtr:    return Operand{get<double>(a) + *get<double*>(b)};
        case Operand::Type::DatePtr:      return Operand{(*get<QDate*    >(b)).addDays(daysToDays(get<double>(a)))};
        case Operand::Type::DateTimePtr:  return Operand{(*get<QDateTime*>(b)).addSecs(daysToSecs(get<double>(a)))};
        case Operand::Type::TimePtr:      return Operand{(*get<QTime*>(b)).addSecs(hoursToSecs(get<double>(a)))};
        case Operand::Type::BareDatePtr:  return Operand{(*get<BareDate* >(b)).addDays(daysToDays(get<double>(a)))};
        case Operand::Type::BareDateTimePtr: return Operand{(*get<BareDateTime*>(b)).addDays(daysToDays(get<double>(a)))};
        case Operand::Type::StringPtr:    return Operand{QString::number(get<double>(a)) + *get<QString*>(b)};
        }
        break;
    case Operand::Type::DoublePtr:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{*get<double*>(a) + get<int>(b)};
        case Operand::Type::Double:       return Operand{*get<double*>(a) + get<double>(b)};
        case Operand::Type::Date:         return Operand{get<QDate    >(b).addDays(daysToDays(*get<double*>(a)))};
        case Operand::Type::DateTime:     return Operand{get<QDateTime>(b).addSecs(daysToSecs(*get<double*>(a)))};
        case Operand::Type::Time:         return Operand{get<QTime>(b).addSecs(hoursToSecs(*get<double*>(a)))};
        case Operand::Type::BareDate:     return Operand{get<BareDate    >(b).addDays(daysToDays(*get<double*>(a)))};
        case Operand::Type::BareDateTime: return Operand{get<BareDateTime>(b).addDays(daysToDays(*get<double*>(a)))};
        case Operand::Type::String:       return Operand{QString::number(*get<double*>(a)) + get<QString>(b)};
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{*get<double*>(a) + *get<int*>(b)};
        case Operand::Type::DoublePtr:    return Operand{*get<double*>(a) + *get<double*>(b)};
        case Operand::Type::DatePtr:      return Operand{(*get<QDate*    >(b)).addDays(daysToDays(*get<double*>(a)))};
        case Operand::Type::DateTimePtr:  return Operand{(*get<QDateTime*>(b)).addSecs(daysToSecs(*get<double*>(a)))};
        case Operand::Type::TimePtr:      return Operand{(*get<QTime*>(b)).addSecs(hoursToSecs(*get<double*>(a)))};
        case Operand::Type::BareDatePtr:  return Operand{(*get<BareDate* >(b)).addDays(daysToDays(*get<double*>(a)))};
        case Operand::Type::BareDateTimePtr: return Operand{(*get<BareDateTime*>(b)).addDays(daysToDays(*get<double*>(a)))};
        case Operand::Type::StringPtr:    return Operand{QString::number(*get<double*>(a)) + *get<QString*>(b)};
        }
        break;
    case Operand::Type::Date:
    case Operand::Type::DatePtr:
    case Operand::Type::DateTime:
    case Operand::Type::DateTimePtr:
    case Operand::Type::Time:
    case Operand::Type::TimePtr:
    case Operand::Type::BareDate:
    case Operand::Type::BareDatePtr:
    case Operand::Type::BareDateTime:
    case Operand::Type::BareDateTimePtr:
        switch(b.type()) {
        case Operand::Type::Int:          return add(b,a);
        case Operand::Type::Double:       return add(b,a);
        case Operand::Type::IntPtr:       return add(b,a);
        case Operand::Type::DoublePtr:    return add(b,a);
        case Operand::Type::String:       return add(b,a);
        case Operand::Type::StringPtr:    return add(b,a);
        default: ;
        }
        break;
    case Operand::Type::String:
    case Operand::Type::StringPtr:
        return Operand{get<QString>(a) + b.toString()};
    }
    ThrowException("Incompatile operands for addition").
            value(a.toString() + " + " + b.toString()).
            value2(a.typeName() + " + " + b.typeName());
}

Operand subtract(const Operand &a, const Operand &b) {
    switch(a.type()) {
    case Operand::Type::Bool:
    case Operand::Type::BoolPtr:
    case Operand::Type::Char:
    case Operand::Type::CharPtr:
        break;
    case Operand::Type::Int:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{get<int>(a) - get<int>(b)};
        case Operand::Type::Double:       return Operand{get<int>(a) - get<double>(b)};
        case Operand::Type::Date:
        case Operand::Type::DateTime:
        case Operand::Type::Time:
        case Operand::Type::BareDate:
        case Operand::Type::BareDateTime:
        case Operand::Type::String:
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{get<int>(a) - *get<int*>(b)};
        case Operand::Type::DoublePtr:    return Operand{get<int>(a) - *get<double*>(b)};
        case Operand::Type::DatePtr:
        case Operand::Type::DateTimePtr:
        case Operand::Type::TimePtr:
        case Operand::Type::BareDatePtr:
        case Operand::Type::BareDateTimePtr:
        case Operand::Type::StringPtr:   break;
        }
        break;
    case Operand::Type::IntPtr:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{*get<int*>(a) - get<int>(b)};
        case Operand::Type::Double:       return Operand{*get<int*>(a) - get<double>(b)};
        case Operand::Type::Date:
        case Operand::Type::DateTime:
        case Operand::Type::Time:
        case Operand::Type::BareDate:
        case Operand::Type::BareDateTime:
        case Operand::Type::String:
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{*get<int*>(a) - *get<int*>(b)};
        case Operand::Type::DoublePtr:    return Operand{*get<int*>(a) - *get<double*>(b)};
        case Operand::Type::DatePtr:
        case Operand::Type::DateTimePtr:
        case Operand::Type::TimePtr:
        case Operand::Type::BareDatePtr:
        case Operand::Type::BareDateTimePtr:
        case Operand::Type::StringPtr:   break;
        }
        break;
    case Operand::Type::Double:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{get<double>(a) - get<int>(b)};
        case Operand::Type::Double:       return Operand{get<double>(a) - get<double>(b)};
        case Operand::Type::Date:
        case Operand::Type::DateTime:
        case Operand::Type::Time:
        case Operand::Type::BareDate:
        case Operand::Type::BareDateTime:
        case Operand::Type::String:
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{get<double>(a) - *get<int*>(b)};
        case Operand::Type::DoublePtr:    return Operand{get<double>(a) - *get<double*>(b)};
        case Operand::Type::DatePtr:
        case Operand::Type::DateTimePtr:
        case Operand::Type::TimePtr:
        case Operand::Type::BareDatePtr:
        case Operand::Type::BareDateTimePtr:
        case Operand::Type::StringPtr:   break;
        }
        break;
    case Operand::Type::DoublePtr:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{*get<double*>(a) - get<int>(b)};
        case Operand::Type::Double:       return Operand{*get<double*>(a) - get<double>(b)};
        case Operand::Type::Date:
        case Operand::Type::DateTime:
        case Operand::Type::Time:
        case Operand::Type::BareDate:
        case Operand::Type::BareDateTime:
        case Operand::Type::String:
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{*get<double*>(a) - *get<int*>(b)};
        case Operand::Type::DoublePtr:    return Operand{*get<double*>(a) - *get<double*>(b)};
        case Operand::Type::DatePtr:
        case Operand::Type::DateTimePtr:
        case Operand::Type::TimePtr:
        case Operand::Type::BareDatePtr:
        case Operand::Type::BareDateTimePtr:
        case Operand::Type::StringPtr:   break;
        }
        break;
    case Operand::Type::Date:
        switch(b.type()) {
        case Operand::Type::Int:          return Operand{get<QDate>(a).addDays(-get<int>(b))};
        case Operand::Type::IntPtr:       return Operand{get<QDate>(a).addDays(-(*get<int*>(b)))};
        case Operand::Type::Double:       return Operand{get<QDate>(a).addDays(-daysToDays(get<double>(b)))};
        case Operand::Type::DoublePtr:    return Operand{get<QDate>(a).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Operand::Type::DatePtr:
        switch(b.type()) {
        case Operand::Type::Int:          return Operand{(*get<QDate*>(a)).addDays(-get<int>(b))};
        case Operand::Type::IntPtr:       return Operand{(*get<QDate*>(a)).addDays(-(*get<int*>(b)))};
        case Operand::Type::Double:       return Operand{(*get<QDate*>(a)).addDays(-daysToDays(get<double>(b)))};
        case Operand::Type::DoublePtr:    return Operand{(*get<QDate*>(a)).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Operand::Type::DateTime:
        switch(b.type()) {
        case Operand::Type::Int:          return Operand{get<QDateTime>(a).addDays(-get<int>(b))};
        case Operand::Type::IntPtr:       return Operand{get<QDateTime>(a).addDays(-(*get<int*>(b)))};
        case Operand::Type::Double:       return Operand{get<QDateTime>(a).addDays(-daysToDays(get<double>(b)))};
        case Operand::Type::DoublePtr:    return Operand{get<QDateTime>(a).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Operand::Type::DateTimePtr:
        switch(b.type()) {
        case Operand::Type::Int:          return Operand{(*get<QDateTime*>(a)).addDays(-get<int>(b))};
        case Operand::Type::IntPtr:       return Operand{(*get<QDateTime*>(a)).addDays(-(*get<int*>(b)))};
        case Operand::Type::Double:       return Operand{(*get<QDateTime*>(a)).addDays(-daysToDays(get<double>(b)))};
        case Operand::Type::DoublePtr:    return Operand{(*get<QDateTime*>(a)).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Operand::Type::Time:
        switch(b.type()) {
        case Operand::Type::Int:          return Operand{get<QTime>(a).addSecs(3600*get<int>(b))};
        case Operand::Type::IntPtr:       return Operand{get<QTime>(a).addSecs(3600*(*get<int*>(b)))};
        case Operand::Type::Double:       return Operand{get<QTime>(a).addSecs(hoursToSecs(get<double>(b)))};
        case Operand::Type::DoublePtr:    return Operand{get<QTime>(a).addSecs(hoursToSecs(*get<double*>(b)))};
        default: ;
        }
        break;
    case Operand::Type::TimePtr:
        switch(b.type()) {
        case Operand::Type::Int:          return Operand{(*get<QTime*>(a)).addSecs(3600*get<int>(b))};
        case Operand::Type::IntPtr:       return Operand{(*get<QTime*>(a)).addSecs(3600*(*get<int*>(b)))};
        case Operand::Type::Double:       return Operand{(*get<QTime*>(a)).addSecs(hoursToSecs(get<double>(b)))};
        case Operand::Type::DoublePtr:    return Operand{(*get<QTime*>(a)).addSecs(hoursToSecs(*get<double*>(b)))};
        default: ;
        }
        break;
    case Operand::Type::BareDate:
        switch(b.type()) {
        case Operand::Type::Int:          return Operand{get<BareDate>(a).addDays(-get<int>(b))};
        case Operand::Type::IntPtr:       return Operand{get<BareDate>(a).addDays(-(*get<int*>(b)))};
        case Operand::Type::Double:       return Operand{get<BareDate>(a).addDays(-daysToDays(get<double>(b)))};
        case Operand::Type::DoublePtr:    return Operand{get<BareDate>(a).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Operand::Type::BareDatePtr:
        switch(b.type()) {
        case Operand::Type::Int:          return Operand{(*get<BareDate*>(a)).addDays(-get<int>(b))};
        case Operand::Type::IntPtr:       return Operand{(*get<BareDate*>(a)).addDays(-(*get<int*>(b)))};
        case Operand::Type::Double:       return Operand{(*get<BareDate*>(a)).addDays(-daysToDays(get<double>(b)))};
        case Operand::Type::DoublePtr:    return Operand{(*get<BareDate*>(a)).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Operand::Type::BareDateTime:
        switch(b.type()) {
        case Operand::Type::Int:          return Operand{get<BareDateTime>(a).addDays(-get<int>(b))};
        case Operand::Type::IntPtr:       return Operand{get<BareDateTime>(a).addDays(-(*get<int*>(b)))};
        case Operand::Type::Double:       return Operand{get<BareDateTime>(a).addDays(-daysToDays(get<double>(b)))};
        case Operand::Type::DoublePtr:    return Operand{get<BareDateTime>(a).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Operand::Type::BareDateTimePtr:
        switch(b.type()) {
        case Operand::Type::Int:          return Operand{(*get<BareDateTime*>(a)).addDays(-get<int>(b))};
        case Operand::Type::IntPtr:       return Operand{(*get<BareDateTime*>(a)).addDays(-(*get<int*>(b)))};
        case Operand::Type::Double:       return Operand{(*get<BareDateTime*>(a)).addDays(-daysToDays(get<double>(b)))};
        case Operand::Type::DoublePtr:    return Operand{(*get<BareDateTime*>(a)).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Operand::Type::String:
    case Operand::Type::StringPtr: break;
    }
    ThrowException("Incompatile operands for subtraction").
            value(a.toString() + " + " + b.toString()).
            value2(a.typeName() + " + " + b.typeName());
}

Operand multiply(const Operand &a, const Operand &b) {
    switch(a.type()) {
    case Operand::Type::Bool:
    case Operand::Type::BoolPtr:
    case Operand::Type::Char:
    case Operand::Type::CharPtr:
        break;
    case Operand::Type::Int:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{get<int>(a) * get<int>(b)};
        case Operand::Type::Double:       return Operand{get<int>(a) * get<double>(b)};
        case Operand::Type::Date:
        case Operand::Type::DateTime:
        case Operand::Type::Time:
        case Operand::Type::BareDate:
        case Operand::Type::BareDateTime:
        case Operand::Type::String:
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{get<int>(a) * (*get<int*>(b))};
        case Operand::Type::DoublePtr:    return Operand{get<int>(a) * (*get<double*>(b))};
        case Operand::Type::DatePtr:
        case Operand::Type::DateTimePtr:
        case Operand::Type::TimePtr:
        case Operand::Type::BareDatePtr:
        case Operand::Type::BareDateTimePtr:
        case Operand::Type::StringPtr:   break;
        }
        break;
    case Operand::Type::IntPtr:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{*get<int*>(a) * get<int>(b)};
        case Operand::Type::Double:       return Operand{*get<int*>(a) * get<double>(b)};
        case Operand::Type::Date:
        case Operand::Type::DateTime:
        case Operand::Type::Time:
        case Operand::Type::BareDate:
        case Operand::Type::BareDateTime:
        case Operand::Type::String:
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{*get<int*>(a) * (*get<int*>(b))};
        case Operand::Type::DoublePtr:    return Operand{*get<int*>(a) * (*get<double*>(b))};
        case Operand::Type::DatePtr:
        case Operand::Type::DateTimePtr:
        case Operand::Type::TimePtr:
        case Operand::Type::BareDatePtr:
        case Operand::Type::BareDateTimePtr:
        case Operand::Type::StringPtr:   break;
        }
        break;
    case Operand::Type::Double:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{get<double>(a) * get<int>(b)};
        case Operand::Type::Double:       return Operand{get<double>(a) * get<double>(b)};
        case Operand::Type::Date:
        case Operand::Type::DateTime:
        case Operand::Type::Time:
        case Operand::Type::BareDate:
        case Operand::Type::BareDateTime:
        case Operand::Type::String:
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{get<double>(a) * (*get<int*>(b))};
        case Operand::Type::DoublePtr:    return Operand{get<double>(a) * (*get<double*>(b))};
        case Operand::Type::DatePtr:
        case Operand::Type::DateTimePtr:
        case Operand::Type::TimePtr:
        case Operand::Type::BareDatePtr:
        case Operand::Type::BareDateTimePtr:
        case Operand::Type::StringPtr:   break;
        }
        break;
    case Operand::Type::DoublePtr:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{*get<double*>(a) * get<int>(b)};
        case Operand::Type::Double:       return Operand{*get<double*>(a) * get<double>(b)};
        case Operand::Type::Date:
        case Operand::Type::DateTime:
        case Operand::Type::Time:
        case Operand::Type::BareDate:
        case Operand::Type::BareDateTime:
        case Operand::Type::String:
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{*get<double*>(a) * (*get<int*>(b))};
        case Operand::Type::DoublePtr:    return Operand{*get<double*>(a) * (*get<double*>(b))};
        case Operand::Type::DatePtr:
        case Operand::Type::DateTimePtr:
        case Operand::Type::TimePtr:
        case Operand::Type::BareDatePtr:
        case Operand::Type::BareDateTimePtr:
        case Operand::Type::StringPtr:   break;
        }
        break;
    case Operand::Type::Date:
    case Operand::Type::DatePtr:
    case Operand::Type::DateTime:
    case Operand::Type::DateTimePtr:
    case Operand::Type::Time:
    case Operand::Type::TimePtr:
    case Operand::Type::BareDate:
    case Operand::Type::BareDatePtr:
    case Operand::Type::BareDateTime:
    case Operand::Type::BareDateTimePtr:
    case Operand::Type::String:
    case Operand::Type::StringPtr: break;
    }
    ThrowException("Incompatile operands for multiplication").
            value(a.toString() + " + " + b.toString()).
            value2(a.typeName() + " + " + b.typeName());
}

Operand divide(const Operand &a, const Operand &b) {
    switch(a.type()) {
    case Operand::Type::Bool:
    case Operand::Type::BoolPtr:
    case Operand::Type::Char:
    case Operand::Type::CharPtr:
        break;
    case Operand::Type::Int:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{get<int>(a) / numeric_cast<double>(get<int>(b))};
        case Operand::Type::Double:       return Operand{get<int>(a) / get<double>(b)};
        case Operand::Type::Date:
        case Operand::Type::DateTime:
        case Operand::Type::Time:
        case Operand::Type::BareDate:
        case Operand::Type::BareDateTime:
        case Operand::Type::String:
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{get<int>(a) / numeric_cast<double>(*get<int*>(b))};
        case Operand::Type::DoublePtr:    return Operand{get<int>(a) / (*get<double*>(b))};
        case Operand::Type::DatePtr:
        case Operand::Type::DateTimePtr:
        case Operand::Type::TimePtr:
        case Operand::Type::BareDatePtr:
        case Operand::Type::BareDateTimePtr:
        case Operand::Type::StringPtr:   break;
        }
        break;
    case Operand::Type::IntPtr:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{*get<int*>(a) / numeric_cast<double>(*get<int*>(b))};
        case Operand::Type::Double:       return Operand{*get<int*>(a) / get<double>(b)};
        case Operand::Type::Date:
        case Operand::Type::DateTime:
        case Operand::Type::Time:
        case Operand::Type::BareDate:
        case Operand::Type::BareDateTime:
        case Operand::Type::String:
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{*get<int*>(a) / numeric_cast<double>(*get<int*>(b))};
        case Operand::Type::DoublePtr:    return Operand{*get<int*>(a) / (*get<double*>(b))};
        case Operand::Type::DatePtr:
        case Operand::Type::DateTimePtr:
        case Operand::Type::TimePtr:
        case Operand::Type::BareDatePtr:
        case Operand::Type::BareDateTimePtr:
        case Operand::Type::StringPtr:   break;
        }
        break;
    case Operand::Type::Double:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{get<double>(a) / get<int>(b)};
        case Operand::Type::Double:       return Operand{get<double>(a) / get<double>(b)};
        case Operand::Type::Date:
        case Operand::Type::DateTime:
        case Operand::Type::Time:
        case Operand::Type::BareDate:
        case Operand::Type::BareDateTime:
        case Operand::Type::String:
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{get<double>(a) / (*get<int*>(b))};
        case Operand::Type::DoublePtr:    return Operand{get<double>(a) / (*get<double*>(b))};
        case Operand::Type::DatePtr:
        case Operand::Type::DateTimePtr:
        case Operand::Type::TimePtr:
        case Operand::Type::BareDatePtr:
        case Operand::Type::BareDateTimePtr:
        case Operand::Type::StringPtr:   break;
        }
        break;
    case Operand::Type::DoublePtr:
        switch(b.type()) {
        case Operand::Type::Bool:
        case Operand::Type::Char:         break;
        case Operand::Type::Int:          return Operand{*get<double*>(a) / get<int>(b)};
        case Operand::Type::Double:       return Operand{*get<double*>(a) / get<double>(b)};
        case Operand::Type::Date:
        case Operand::Type::DateTime:
        case Operand::Type::Time:
        case Operand::Type::BareDate:
        case Operand::Type::BareDateTime:
        case Operand::Type::String:
        case Operand::Type::BoolPtr:
        case Operand::Type::CharPtr:      break;
        case Operand::Type::IntPtr:       return Operand{*get<double*>(a) / (*get<int*>(b))};
        case Operand::Type::DoublePtr:    return Operand{*get<double*>(a) / (*get<double*>(b))};
        case Operand::Type::DatePtr:
        case Operand::Type::DateTimePtr:
        case Operand::Type::TimePtr:
        case Operand::Type::BareDatePtr:
        case Operand::Type::BareDateTimePtr:
        case Operand::Type::StringPtr:   break;
        }
        break;
    case Operand::Type::Date:
    case Operand::Type::DatePtr:
    case Operand::Type::DateTime:
    case Operand::Type::DateTimePtr:
    case Operand::Type::Time:
    case Operand::Type::TimePtr:
    case Operand::Type::BareDate:
    case Operand::Type::BareDatePtr:
    case Operand::Type::BareDateTime:
    case Operand::Type::BareDateTimePtr:
    case Operand::Type::String:
    case Operand::Type::StringPtr: break;
    }
    ThrowException("Incompatile operands for division").
            value(a.toString() + " + " + b.toString()).
            value2(a.typeName() + " + " + b.typeName());
}

}
