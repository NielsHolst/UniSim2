/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "value.h"

namespace base {


void Value::copyValue(const Value &x) {
    switch(type()) {
    case Type::Uninitialized: _variant = x._variant; break;
    case Type::Bool         : changeValue(x.value<bool     >()); break;
    case Type::Int          : changeValue(x.value<int      >()); break;
    case Type::Double       : changeValue(x.value<double   >()); break;
    case Type::String       : changeValue(x.value<QString  >()); break;
    case Type::Date         : changeValue(x.value<QDate    >()); break;
    case Type::Time         : changeValue(x.value<QTime    >()); break;
    case Type::DateTime     : changeValue(x.value<QDateTime>()); break;
    case Type::BareDate     : changeValue(x.value<BareDate >()); break;
    case Type::VecBool      : changeValue(x.value<QVector<bool     >>()); break;
    case Type::VecInt       : changeValue(x.value<QVector<int      >>()); break;
    case Type::VecDouble    : changeValue(x.value<QVector<double   >>()); break;
    case Type::VecString    : changeValue(x.value<QVector<QString  >>()); break;
    case Type::VecDate      : changeValue(x.value<QVector<QDate    >>()); break;
    case Type::VecTime      : changeValue(x.value<QVector<QTime    >>()); break;
    case Type::VecDateTime  : changeValue(x.value<QVector<QDateTime>>()); break;
    case Type::VecBareDate  : changeValue(x.value<QVector<BareDate >>()); break;
    }
}


#define VALUE_PTR(cppType, valueType) \
template <> const cppType* Value::valuePtr() const { \
    if (type() != valueType) \
        ThrowException("Value is not of type " + QString(#cppType)); \
    return std::get<ValueTyped<cppType>>(_variant).valuePtr(); \
}

VALUE_PTR(bool     , Type::Bool)
VALUE_PTR(int      , Type::Int)
VALUE_PTR(double   , Type::Double)
VALUE_PTR(QString  , Type::String)
VALUE_PTR(QDate    , Type::Date)
VALUE_PTR(QTime    , Type::Time)
VALUE_PTR(QDateTime, Type::DateTime)
VALUE_PTR(BareDate , Type::BareDate)
VALUE_PTR(QVector<bool>     , Type::VecBool)
VALUE_PTR(QVector<int>      , Type::VecInt)
VALUE_PTR(QVector<double>   , Type::VecDouble)
VALUE_PTR(QVector<QString>  , Type::VecString)
VALUE_PTR(QVector<QDate>    , Type::VecDate)
VALUE_PTR(QVector<QTime>    , Type::VecTime)
VALUE_PTR(QVector<QDateTime>, Type::VecDateTime)
VALUE_PTR(QVector<BareDate> , Type::VecBareDate)

QString Value::typeName() const {
    switch(type()) {
    case Type::Uninitialized: return "Uninitialized";
    case Type::Bool         : return "Bool";
    case Type::Int          : return "Int";
    case Type::Double       : return "Double";
    case Type::String       : return "String";
    case Type::Date         : return "Date";
    case Type::Time         : return "Time";
    case Type::DateTime     : return "DateTime";
    case Type::BareDate     : return "BareDate";
    case Type::VecBool      : return "VectorOfBool";
    case Type::VecInt       : return "VectorOfInt";
    case Type::VecDouble    : return "VectorOfDouble";
    case Type::VecString    : return "VectorOfString";
    case Type::VecDate      : return "VectorOfDate";
    case Type::VecTime      : return "VectorOfTime";
    case Type::VecDateTime  : return "VectorOfDateTime";
    case Type::VecBareDate  : return "VectorOfBareDate";
    }
    return QString();
}


}
