/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <boost/variant/get.hpp>
#include "bare_date.h"
#include "bare_date_time.h"
#include "expression_operand.h"

using boost::get;
using base::BareDate;
using base::BareDateTime;

namespace expression {

QString Operand::toString() const {
    const Operand &a(*this);
    switch(a.type()) {
    case Operand::Type::Bool:            return QString::number(get<bool>(a));
    case Operand::Type::Char:            return QString(get<char>(a));
    case Operand::Type::Int:             return QString::number(get<int>(a));
    case Operand::Type::Double:          return QString::number(get<double>(a));
    case Operand::Type::Date:            return get<QDate>(a).toString("yyyy/MM/dd");
    case Operand::Type::DateTime:        return get<QDateTime>(a).toString("yyyy/MM/ddThh:mm:ss");
    case Operand::Type::Time:            return get<QTime>(a).toString("hh:mm:ss");
    case Operand::Type::BareDate:        return get<BareDate>(a).toString("MM/dd");
    case Operand::Type::BareDateTime:    return get<BareDateTime>(a).toString("MM/ddThh:mm:ss");
    case Operand::Type::String:          return get<QString>(a);
    case Operand::Type::BoolPtr:         return QString::number(*get<bool*>(a));;
    case Operand::Type::CharPtr:         return QString::number(*get<char*>(a));
    case Operand::Type::IntPtr:          return QString::number(*get<int*>(a));
    case Operand::Type::DoublePtr:       return QString::number(*get<double*>(a));
    case Operand::Type::DatePtr:         return (*get<QDate*>(a)).toString("yyyy/MM/dd");
    case Operand::Type::DateTimePtr:     return (*get<QDateTime*>(a)).toString("yyyy/MM/ddThh:mm:ss");
    case Operand::Type::TimePtr:         return (*get<QTime*>(a)).toString("hh:mm:ss");
    case Operand::Type::BareDatePtr:     return (*get<BareDate*>(a)).toString("MM/dd");
    case Operand::Type::BareDateTimePtr: return (*get<BareDateTime*>(a)).toString("MM/ddThh:mm:ss");
    case Operand::Type::StringPtr:       return (*get<QString*>(a));
    }
    ThrowException("Unexpected exception");
}

}
