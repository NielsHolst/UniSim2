/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QStringList>
#include "convert.h"
#include "value.h"

namespace base {

inline QString aph(bool apostrophed, QString s) {
    return apostrophed ? "\"" + s + "\"" : s;
}

QString Value::asString(bool apostrophed, bool vectorized) const {
    QStringList slist;
    switch(type()) {
    case Type::Uninitialized: break;
    case Type::VecBool      : for (auto x : as<vbool     >()) slist += convert<QString>(x);; break;
    case Type::VecInt       : for (auto x : as<vint      >()) slist += convert<QString>(x);; break;
    case Type::VecDouble    : for (auto x : as<vdouble   >()) slist += convert<QString>(x);; break;
    case Type::VecDate      : for (auto x : as<vQDate    >()) slist += convert<QString>(x);; break;
    case Type::VecTime      : for (auto x : as<vQTime    >()) slist += convert<QString>(x);; break;
    case Type::VecDateTime  : for (auto x : as<vQDateTime>()) slist += convert<QString>(x);; break;
    case Type::VecBareDate  : for (auto x : as<vBareDate >()) slist += convert<QString>(x);; break;

    case Type::String       : slist += aph(apostrophed, as<QString>());
                              break;
    case Type::VecString    : for (QString x : as<vQString  >())
                                slist += aph(apostrophed, x);
                              break;
    case Type::Path         : slist += as<Path>().original();
                              break;
    default                 : slist += as<QString>();
                              break;
    }
    if (isVector()) {
       return vectorized ?  "c(" + slist.join(",") +")" : slist.join("\t");
    }
    else {
       return slist.at(0);
    }
}

void Value::reset() {
    switch(type()) {
    case Type::Uninitialized: break;
    case Type::Bool         : changeValue(bool     ()); break;
    case Type::Int          : changeValue(int      ()); break;
    case Type::Double       : changeValue(double   ()); break;
    case Type::String       : changeValue(QString  ()); break;
    case Type::Date         : changeValue(QDate    ()); break;
    case Type::Time         : changeValue(QTime    ()); break;
    case Type::DateTime     : changeValue(QDateTime()); break;
    case Type::BareDate     : changeValue(BareDate ()); break;
    case Type::Path         : changeValue(Path     ()); break;
    case Type::VecBool      : changeValue(vbool      (size())); break;
    case Type::VecInt       : changeValue(vint       (size())); break;
    case Type::VecDouble    : changeValue(vdouble    (size())); break;
    case Type::VecString    : changeValue(vQString   (size())); break;
    case Type::VecDate      : changeValue(vQDate     (size())); break;
    case Type::VecTime      : changeValue(vQTime     (size())); break;
    case Type::VecDateTime  : changeValue(vQDateTime (size())); break;
    case Type::VecBareDate  : changeValue(vBareDate  (size())); break;
    }
}

int Value::size() const {
    switch(type()) {
    case Type::Uninitialized: break;
    case Type::VecBool      : return as<vbool     >().size(); break;
    case Type::VecInt       : return as<vint      >().size(); break;
    case Type::VecDouble    : return as<vdouble   >().size(); break;
    case Type::VecString    : return as<vQString  >().size(); break;
    case Type::VecDate      : return as<vQDate    >().size(); break;
    case Type::VecTime      : return as<vQTime    >().size(); break;
    case Type::VecDateTime  : return as<vQDateTime>().size(); break;
    case Type::VecBareDate  : return as<vBareDate >().size(); break;
    default: return 1;
    }
    return 0;
}

QString Value::outputFormat() const {
    switch(type()) {
    case Type::Date:
    case Type::VecDate: return "ymd"; break;
    case Type::Time:
    case Type::VecTime: return "HMS"; break;
    case Type::DateTime:
    case Type::VecDateTime: return "ymdHMS"; break;
    default: ;
    }
    return "NA";
}


void Value::assign(const Value &x) {
    switch(type()) {
    case Type::Uninitialized: _variant = x._variant; break;
    case Type::Bool         : changeValue(x.as<bool     >()); break;
    case Type::Int          : changeValue(x.as<int      >()); break;
    case Type::Double       : changeValue(x.as<double   >()); break;
    case Type::String       : changeValue(x.as<QString  >()); break;
    case Type::Date         : changeValue(x.as<QDate    >()); break;
    case Type::Time         : changeValue(x.as<QTime    >()); break;
    case Type::DateTime     : changeValue(x.as<QDateTime>()); break;
    case Type::BareDate     : changeValue(x.as<BareDate >()); break;
    case Type::Path         : changeValue(x.as<Path     >()); break;
    case Type::VecBool      : changeValue(x.as<QVector<bool     >>()); break;
    case Type::VecInt       : changeValue(x.as<QVector<int      >>()); break;
    case Type::VecDouble    : changeValue(x.as<QVector<double   >>()); break;
    case Type::VecString    : changeValue(x.as<QVector<QString  >>()); break;
    case Type::VecDate      : changeValue(x.as<QVector<QDate    >>()); break;
    case Type::VecTime      : changeValue(x.as<QVector<QTime    >>()); break;
    case Type::VecDateTime  : changeValue(x.as<QVector<QDateTime>>()); break;
    case Type::VecBareDate  : changeValue(x.as<QVector<BareDate >>()); break;
    }
}

bool Value::operator==(const Value &x) const {
    if (type() == x.type())
        switch(type()) {
        case Type::Uninitialized: return true;
        case Type::Bool         : return as<bool      >() == x.as<bool      >();
        case Type::Int          : return as<int       >() == x.as<int       >();
        case Type::Double       : return as<double    >() == x.as<double    >();
        case Type::String       : return as<QString   >() == x.as<QString   >();
        case Type::Date         : return as<QDate     >() == x.as<QDate     >();
        case Type::Time         : return as<QTime     >() == x.as<QTime     >();
        case Type::DateTime     : return as<QDateTime >() == x.as<QDateTime >();
        case Type::BareDate     : return as<BareDate  >() == x.as<BareDate  >();
        case Type::Path         : return as<Path      >().original() == x.as<Path>().original();
        case Type::VecBool      : return as<vbool     >() == x.as<vbool     >();
        case Type::VecInt       : return as<vint      >() == x.as<vint      >();
        case Type::VecDouble    : return as<vdouble   >() == x.as<vdouble   >();
        case Type::VecString    : return as<vQString  >() == x.as<vQString  >();
        case Type::VecDate      : return as<vQDate    >() == x.as<vQDate    >();
        case Type::VecTime      : return as<vQTime    >() == x.as<vQTime    >();
        case Type::VecDateTime  : return as<vQDateTime>() == x.as<vQDateTime>();
        case Type::VecBareDate  : return as<vQDate    >() == x.as<vQDate    >();
        }
    return false;
}

#define VALUE_PTR(cppType, valueType) \
template <> const cppType* Value::constPtr() const { \
    if (type() != valueType) \
        ThrowException("Value is not of type " + QString(#cppType)).value(typeName()); \
    return std::get<ValueTyped<cppType>>(_variant).constPtr(); \
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
    return typeName(type());
}

QString Value::typeName(Type type){
    switch(type) {
    case Type::Uninitialized: return "Uninitialized";
    case Type::Bool         : return "Bool";
    case Type::Int          : return "Int";
    case Type::Double       : return "Double";
    case Type::String       : return "String";
    case Type::Date         : return "Date";
    case Type::Time         : return "Time";
    case Type::DateTime     : return "DateTime";
    case Type::BareDate     : return "BareDate";
    case Type::Path         : return "Path";
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
