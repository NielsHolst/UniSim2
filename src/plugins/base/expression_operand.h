/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef EXPRESSION_OPERAND_H
#define EXPRESSION_OPERAND_H
#include <boost/variant/variant.hpp>
#include <QDate>
#include <QDateTime>
#include <QTime>
#include "bare_date.h"
#include "bare_date_time.h"
#include "exception.h"

#define TYPE_NAME(X) case Type::X: return #X

namespace expression {

using OperandVariant = boost::variant
<
bool,
char,
int,
double,
QDate,
QDateTime,
QTime,
base::BareDate,
base::BareDateTime,
QString,
bool*,
char*,
int*,
double*,
QDate*,
QDateTime*,
QTime*,
base::BareDate*,
base::BareDateTime*,
QString*
>;

class Operand : public OperandVariant
{
public:
    // Inherit constructors and operator=
    using OperandVariant::OperandVariant;
    using OperandVariant::operator=;

    enum class Type
    {
        Bool,
        Char,
        Int,
        Double,
        Date,
        DateTime,
        Time,
        BareDate,
        BareDateTime,
        String,
        BoolPtr,
        CharPtr,
        IntPtr,
        DoublePtr,
        DatePtr,
        DateTimePtr,
        TimePtr,
        BareDatePtr,
        BareDateTimePtr,
        StringPtr
    };
    Type type() const {
        return static_cast<Type>(which());
    }
    QString typeName() const {
        switch(type()) {
        TYPE_NAME(Bool);
        TYPE_NAME(Char);
        TYPE_NAME(Int);
        TYPE_NAME(Double);
        TYPE_NAME(Date);
        TYPE_NAME(DateTime);
        TYPE_NAME(Time);
        TYPE_NAME(BareDate);
        TYPE_NAME(BareDateTime);
        TYPE_NAME(String);
        TYPE_NAME(BoolPtr);
        TYPE_NAME(CharPtr);
        TYPE_NAME(IntPtr);
        TYPE_NAME(DoublePtr);
        TYPE_NAME(DatePtr);
        TYPE_NAME(DateTimePtr);
        TYPE_NAME(TimePtr);
        TYPE_NAME(BareDatePtr);
        TYPE_NAME(BareDateTimePtr);
        TYPE_NAME(StringPtr);
        }
        ThrowException("Unexpected exception");
        return QString();
    }
    QString toString() const;
};

}
#endif
