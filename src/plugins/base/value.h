/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_VALUE_H
#define BASE_VALUE_H

#include <variant>
#include <QDate>
#include <QDateTime>
#include <QTime>
#include "bare_date.h"
#include "exception.h"
#include "value_typed.h"

namespace base {

class Value {
public:

    enum class Type { // Add same as vectors + a QVector<Value*> for heterogenous types
        Uninitialized,
        Bool,
        Int,
        Double,
        String,
        Date,
        Time,
        DateTime,
        BareDate,
        VecBool,
        VecInt,
        VecDouble,
        VecString,
        VecDate,
        VecTime,
        VecDateTime,
        VecBareDate
    };

    Value(            ) {                }
    Value(bool       x) { initialize(x); }
    Value(int        x) { initialize(x); }
    Value(double     x) { initialize(x); }
    Value(QString    x) { initialize(x); }
    Value(QDate      x) { initialize(x); }
    Value(QTime      x) { initialize(x); }
    Value(QDateTime  x) { initialize(x); }
    Value(BareDate   x) { initialize(x); }
    Value(const char *x) { initialize(QString(x)); }

    Value(bool      *x) { initialize(x); }
    Value(int       *x) { initialize(x); }
    Value(double    *x) { initialize(x); }
    Value(QString   *x) { initialize(x); }
    Value(QDate     *x) { initialize(x); }
    Value(QTime     *x) { initialize(x); }
    Value(QDateTime *x) { initialize(x); }
    Value(BareDate  *x) { initialize(x); }

    Value(QVector<bool>       x) { initialize(x); }
    Value(QVector<int>        x) { initialize(x); }
    Value(QVector<double>     x) { initialize(x); }
    Value(QVector<QString>    x) { initialize(x); }
    Value(QVector<QDate>      x) { initialize(x); }
    Value(QVector<QTime>      x) { initialize(x); }
    Value(QVector<QDateTime>  x) { initialize(x); }
    Value(QVector<BareDate>   x) { initialize(x); }

    Value(QVector<bool>      *x) { initialize(x); }
    Value(QVector<int>       *x) { initialize(x); }
    Value(QVector<double>    *x) { initialize(x); }
    Value(QVector<QString>   *x) { initialize(x); }
    Value(QVector<QDate>     *x) { initialize(x); }
    Value(QVector<QTime>     *x) { initialize(x); }
    Value(QVector<QDateTime> *x) { initialize(x); }
    Value(QVector<BareDate>  *x) { initialize(x); }

    template <class T> void initialize(T *variable)
    // Link to outside variable
    {
        _variant = ValueTyped<T>(variable);
    }

    template <class T> void initialize(T value)
    // Link to inside variable
    {
        _variant = ValueTyped<T>(value);
    }

    template <class U> void changeValue(U value);
    // Set to value with compatible type U

    template <class T> const T* valuePtr() const;
    // Return pointer in native type T

    template <class U> U value() const;
    // Get value in compatible type U

    Type type() const
    // Return the value's type
    {
        return static_cast<Type>(_variant.index());
    }

    bool isVector() const
    // Return if this is a vectored type
    {
        return (type() >= Type::VecBool);
    }

    QString typeName() const;
    // Return the value's type name

    void copyValue(const Value &x);
    Value(const Value &x)            { copyValue(x); }
    Value& operator=(const Value &x) { copyValue(x);  return *this; }
    // Copy from another Value but keep my own type

private:
    std::variant<
        std::monostate,
        ValueTyped<bool>,
        ValueTyped<int>,
        ValueTyped<double>,
        ValueTyped<QString>,
        ValueTyped<QDate>,
        ValueTyped<QTime>,
        ValueTyped<QDateTime>,
        ValueTyped<BareDate>,
        ValueTyped<QVector<bool>>,
        ValueTyped<QVector<int>>,
        ValueTyped<QVector<double>>,
        ValueTyped<QVector<QString>>,
        ValueTyped<QVector<QDate>>,
        ValueTyped<QVector<QTime>>,
        ValueTyped<QVector<QDateTime>>,
        ValueTyped<QVector<BareDate>>
    >
    _variant; // Starts out uninitialised (= monostate)
};

template <class U> void Value::changeValue(U value)
{
    switch(type()) {
    case Type::Uninitialized:
        ThrowException("Value is uninitialized");
        break;
    case Type::Bool:
        std::get<ValueTyped<bool>>(_variant).changeValue(value);
        break;
    case Type::Int:
        std::get<ValueTyped<int>>(_variant).changeValue(value);
        break;
    case Type::Double:
        std::get<ValueTyped<double>>(_variant).changeValue(value);
        break;
    case Type::String:
        std::get<ValueTyped<QString>>(_variant).changeValue(value);
        break;
    case Type::Date:
        std::get<ValueTyped<QDate>>(_variant).changeValue(value);
        break;
    case Type::Time:
        std::get<ValueTyped<QTime>>(_variant).changeValue(value);
        break;
    case Type::DateTime:
        std::get<ValueTyped<QDateTime>>(_variant).changeValue(value);
        break;
    case Type::BareDate:
        std::get<ValueTyped<BareDate>>(_variant).changeValue(value);
        break;
    case Type::VecBool:
        std::get<ValueTyped<QVector<bool>>>(_variant).changeValue(value);
        break;
    case Type::VecInt:
        std::get<ValueTyped<QVector<int>>>(_variant).changeValue(value);
        break;
    case Type::VecDouble:
        std::get<ValueTyped<QVector<double>>>(_variant).changeValue(value);
        break;
    case Type::VecString:
        std::get<ValueTyped<QVector<QString>>>(_variant).changeValue(value);
        break;
    case Type::VecDate:
        std::get<ValueTyped<QVector<QDate>>>(_variant).changeValue(value);
        break;
    case Type::VecTime:
        std::get<ValueTyped<QVector<QTime>>>(_variant).changeValue(value);
        break;
    case Type::VecDateTime:
        std::get<ValueTyped<QVector<QDateTime>>>(_variant).changeValue(value);
        break;
    case Type::VecBareDate:
        std::get<ValueTyped<QVector<BareDate>>>(_variant).changeValue(value);
        break;
    }
}


template <> const bool*      Value::valuePtr() const;
template <> const int*       Value::valuePtr() const;
template <> const double*    Value::valuePtr() const;
template <> const QString*   Value::valuePtr() const;
template <> const QDate*     Value::valuePtr() const;
template <> const QTime*     Value::valuePtr() const;
template <> const QDateTime* Value::valuePtr() const;
template <> const BareDate*  Value::valuePtr() const;
template <> const QVector<bool>*      Value::valuePtr() const;
template <> const QVector<int>*       Value::valuePtr() const;
template <> const QVector<double>*    Value::valuePtr() const;
template <> const QVector<QString>*   Value::valuePtr() const;
template <> const QVector<QDate>*     Value::valuePtr() const;
template <> const QVector<QTime>*     Value::valuePtr() const;
template <> const QVector<QDateTime>* Value::valuePtr() const;
template <> const QVector<BareDate>*  Value::valuePtr() const;

template <class U> U Value::value() const
{
    switch(type()) {
    case Type::Uninitialized:
        ThrowException("Value is uninitialized");
        break;
    case Type::Bool:
        return std::get<ValueTyped<bool>>(_variant).value<U>();
    case Type::Int:
        return std::get<ValueTyped<int>>(_variant).value<U>();
    case Type::Double:
        return std::get<ValueTyped<double>>(_variant).value<U>();
    case Type::String:
        return std::get<ValueTyped<QString>>(_variant).value<U>();
    case Type::Date:
        return std::get<ValueTyped<QDate>>(_variant).value<U>();
    case Type::Time:
        return std::get<ValueTyped<QTime>>(_variant).value<U>();
    case Type::DateTime:
        return std::get<ValueTyped<QDateTime>>(_variant).value<U>();
    case Type::BareDate:
        return std::get<ValueTyped<BareDate>>(_variant).value<U>();
    case Type::VecBool:
        return std::get<ValueTyped<QVector<bool>>>(_variant).value<U>();
    case Type::VecInt:
        return std::get<ValueTyped<QVector<int>>>(_variant).value<U>();
    case Type::VecDouble:
        return std::get<ValueTyped<QVector<double>>>(_variant).value<U>();
    case Type::VecString:
        return std::get<ValueTyped<QVector<QString>>>(_variant).value<U>();
    case Type::VecDate:
        return std::get<ValueTyped<QVector<QDate>>>(_variant).value<U>();
    case Type::VecTime:
        return std::get<ValueTyped<QVector<QTime>>>(_variant).value<U>();
    case Type::VecDateTime:
        return std::get<ValueTyped<QVector<QDateTime>>>(_variant).value<U>();
    case Type::VecBareDate:
        return std::get<ValueTyped<QVector<BareDate>>>(_variant).value<U>();
    }
    return U();
}



}
#endif
