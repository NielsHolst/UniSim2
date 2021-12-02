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
using boost::numeric_cast;

namespace operate {

// Exponentiation by squaring
// stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int

int ipow(int base, int exp)
{
    // Special cases
    bool isNegative = (base<0 && exp%2==1);
    if (base == 0)
        if (exp == 0)
            ThrowException("0^0 is illegal");
        else
            return 0;
    else if (exp == 0)
        return 1;
    // Exponentiate
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }
    // Correct for sign
    return isNegative ? -result : result;
}

//
// Date and time operators
//

inline QDate operator+(const QDate &date, double x) { return date.addDays( convert<int>(x)); }
inline QDate operator-(const QDate &date, double x) { return date.addDays(-convert<int>(x)); }
inline int   operator-(const QDate &a, const QDate &b) { return numeric_cast<int>(b.daysTo(a)); }
inline QDate operator+(double x, const QDate &date) { return date + x; }

inline QTime  operator+(const QTime &time, double x) { return time.addMSecs( convert<int>(3600*1000*x)); }
inline QTime  operator-(const QTime &time, double x) { return time.addMSecs(-convert<int>(3600*1000*x)); }
inline double operator-(const QTime &a, const QTime &b) { return b.secsTo(a)/3600.; }
inline QTime  operator+(double x, const QTime &time) { return time + x; }

inline QDateTime operator+(const QDateTime &dt, int x) { return dt.addDays( x); }
inline QDateTime operator-(const QDateTime &dt, int x) { return dt.addDays(-x); }
inline double    operator-(const QDateTime &a, const QDateTime &b) { return b.secsTo(a)/3600./24.; }
inline QDateTime operator+(int x, const QDateTime &dt) { return dt + x; }

inline QDateTime operator+(const QDateTime &dt, double x) {
    QDateTime y(dt);
    return y.addDays(convert<int>(floor(x))).addSecs( convert<int>(24*3600*(x-floor(x))));
}
inline QDateTime operator-(const QDateTime &dt, double x) {
    QDateTime y(dt);
    return y.addDays(convert<int>(floor(x))).addSecs(-convert<int>(24*3600*(x-floor(x))));
}
inline QDateTime operator+(double x, const QDateTime &dt) { return dt + x; }

inline BareDate operator+(const BareDate &date, double x) { return date.addBareDays( convert<int>(x)); }
inline BareDate operator-(const BareDate &date, double x) { return date.addBareDays(-convert<int>(x)); }
inline int      operator-(const BareDate &a, const BareDate &b) { return numeric_cast<int>(b.daysTo(a)); }
inline BareDate operator+(double x, const BareDate &date) { return date + x; }

//
// Template functions for vector-scalar and vector-vector operations
//

template <class T, class U, class V> Value add_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) + b;
    return Value(result);
}

template <class T, class U, class V> Value subtract_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) - b;
    return Value(result);
}

template <class T, class U, class V> Value multiply_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) * b;
    return Value(result);
}

template <class T, class U, class V> Value divide_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) / b;
    return Value(result);
}

template <class T, class U, class V> Value exp_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = pow(*A++,  b);
    return Value(result);
}

template <class T> Value exp_vector_scalar(vint a, int b) {
    auto A = a.data();
    int n = a.size();
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = ipow(*A++,  b);
    return Value(result);
}

Value and_vector_scalar(vbool a, bool b) {
    auto A = a.data();
    int n = a.size();
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = *A++ && b;
    return Value(result);
}

Value or_vector_scalar(vbool a, bool b) {
    auto A = a.data();
    int n = a.size();
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = *A++ || b;
    return Value(result);
}

template <class T, class U, class V> Value add_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) + (*B++);
    return Value(result);
}

template <class T, class U, class V> Value subtract_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) - (*B++);
    return Value(result);
}

template <class T, class U, class V> Value multiply_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) * (*B++);
    return Value(result);
}

template <class T, class U, class V> Value divide_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) / (*B++);
    return Value(result);
}

template <class T, class U, class V> Value exp_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = pow(*A++,  *B++);
    return Value(result);
}

template <class T> Value exp_vector_vector(vint a, vint b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = ipow(*A++,  *B++);
    return Value(result);
}

Value and_vector_vector(vbool a, vbool b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = *A++ && *B++;
    return Value(result);
}

Value or_vector_vector(vbool a, vbool b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = *A++ || *B++;
    return Value(result);
}

 Value not_vector(vbool a) {
    auto A = a.data();
    int n = a.size();
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = !*A++;
    return Value(result);
}

template <class T> Value negate_vector(T a) {
    auto A = a.data();
    int n = a.size();
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = -*A++;
    return Value(result);
}


//
// Addition
//

Value add(const Value &a, const Value &b) {
    using Type = Value::Type;

    if (!a.isVector() && b.isVector()) return add(b, a);

    switch (a.type()) {
    case Type::Uninitialized :
        break;
    case Type::Bool:
        break;
    case Type::Int:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<int>() + b.as<int>());
        case Type::Double  : return Value(a.as<int>() + b.as<double>());
        case Type::Date    : return Value(a.as<int>() + b.as<QDate>());
        case Type::Time    : return Value(a.as<int>() + b.as<QTime>());
        case Type::DateTime: return Value(a.as<int>() + b.as<QDateTime>());
        case Type::BareDate: return Value(a.as<int>() + b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Double:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<double>() + b.as<int>());
        case Type::Double  : return Value(a.as<double>() + b.as<double>());
        case Type::Date    : return Value(a.as<double>() + b.as<QDate>());
        case Type::Time    : return Value(a.as<double>() + b.as<QTime>());
        case Type::DateTime: return Value(a.as<double>() + b.as<QDateTime>());
        case Type::BareDate: return Value(a.as<double>() + b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::String:
        switch (b.type()) {
        case Type::String  : return Value(a.as<QString>() + b.as<QString>());
        default            : ;
        }
        break;
    case Type::Date:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<QDate>() + b.as<int>());
        case Type::Double  : return Value(a.as<QDate>() + b.as<double>());
        default            : ;
        }
        break;
    case Type::Time:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<QTime>() + b.as<int>());
        case Type::Double  : return Value(a.as<QTime>() + b.as<double>());
        default            : ;
        }
        break;
    case Type::DateTime:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<QDateTime>() + b.as<int>());
        case Type::Double  : return Value(a.as<QDateTime>() + b.as<double>());
        default            : ;
        }
        break;
    case Type::BareDate:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<BareDate>() + b.as<int>());
        case Type::Double  : return Value(a.as<BareDate>() + b.as<double>());
        default            : ;
        }
        break;
    //
    // Vector
    //
    case Type::VecBool:
        break;
    case Type::VecInt:
        switch (b.type()) {
        case Type::Int     : return add_vector_scalar<vint>      (a.as<vint>(), b.as<int>());
        case Type::Double  : return add_vector_scalar<vdouble>   (a.as<vint>(), b.as<double>());
        case Type::Date    : return add_vector_scalar<vQDate>    (a.as<vint>(), b.as<QDate>());
        case Type::Time    : return add_vector_scalar<vQTime>    (a.as<vint>(), b.as<QTime>());
        case Type::DateTime: return add_vector_scalar<vQDateTime>(a.as<vint>(), b.as<QDateTime>());
        case Type::BareDate: return add_vector_scalar<vBareDate> (a.as<vint>(), b.as<BareDate>());

        case Type::VecInt     : return add_vector_vector<vint>      (a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return add_vector_vector<vdouble>   (a.as<vint>(), b.as<vdouble>());
        case Type::VecDate    : return add_vector_vector<vQDate>    (a.as<vint>(), b.as<vQDate>());
        case Type::VecTime    : return add_vector_vector<vQTime>    (a.as<vint>(), b.as<vQTime>());
        case Type::VecDateTime: return add_vector_vector<vQDateTime>(a.as<vint>(), b.as<vQDateTime>());
        case Type::VecBareDate: return add_vector_vector<vBareDate> (a.as<vint>(), b.as<vBareDate>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        switch (b.type()) {
        case Type::Int     : return add_vector_scalar<vdouble>   (a.as<vdouble>(), b.as<int>());
        case Type::Double  : return add_vector_scalar<vdouble>   (a.as<vdouble>(), b.as<double>());
        case Type::Date    : return add_vector_scalar<vQDate>    (a.as<vdouble>(), b.as<QDate>());
        case Type::Time    : return add_vector_scalar<vQTime>    (a.as<vdouble>(), b.as<QTime>());
        case Type::DateTime: return add_vector_scalar<vQDateTime>(a.as<vdouble>(), b.as<QDateTime>());
        case Type::BareDate: return add_vector_scalar<vBareDate> (a.as<vdouble>(), b.as<BareDate>());

        case Type::VecInt     : return add_vector_vector<vdouble>   (a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return add_vector_vector<vdouble>   (a.as<vdouble>(), b.as<vdouble>());
        case Type::VecDate    : return add_vector_vector<vQDate>    (a.as<vdouble>(), b.as<vQDate>());
        case Type::VecTime    : return add_vector_vector<vQTime>    (a.as<vdouble>(), b.as<vQTime>());
        case Type::VecDateTime: return add_vector_vector<vQDateTime>(a.as<vdouble>(), b.as<vQDateTime>());
        case Type::VecBareDate: return add_vector_vector<vBareDate> (a.as<vdouble>(), b.as<vBareDate>());
        default               : ;
        }
        break;
    case Type::VecString:
        switch (b.type()) {
        case Type::String    : return add_vector_scalar<vQString> (a.as<vQString>(), b.as< QString>());
        case Type::VecString : return add_vector_vector<vQString> (a.as<vQString>(), b.as<vQString>());
        default              : ;
        }
        break;
    case Type::VecDate:
        switch (b.type()) {
        case Type::Int       : return add_vector_scalar<vQDate> (a.as<vQDate>(), b.as<int>());
        case Type::Double    : return add_vector_scalar<vQDate> (a.as<vQDate>(), b.as<double>());
        case Type::VecInt    : return add_vector_vector<vQDate> (a.as<vQDate>(), b.as<vint>());
        case Type::VecDouble : return add_vector_vector<vQDate> (a.as<vQDate>(), b.as<vdouble>());
        default              : ;
        }
        break;
    case Type::VecTime:
        switch (b.type()) {
        case Type::Int       : return add_vector_scalar<vQTime> (a.as<vQTime>(), b.as<int>());
        case Type::Double    : return add_vector_scalar<vQTime> (a.as<vQTime>(), b.as<double>());
        case Type::VecInt    : return add_vector_vector<vQTime> (a.as<vQTime>(), b.as<vint>());
        case Type::VecDouble : return add_vector_vector<vQTime> (a.as<vQTime>(), b.as<vdouble>());
        default            : ;
        }
        break;
    case Type::VecDateTime:
        switch (b.type()) {
        case Type::Int       : return add_vector_scalar<vQDateTime> (a.as<vQDateTime>(), b.as<int>());
        case Type::Double    : return add_vector_scalar<vQDateTime> (a.as<vQDateTime>(), b.as<double>());
        case Type::VecInt    : return add_vector_vector<vQDateTime> (a.as<vQDateTime>(), b.as<vint>());
        case Type::VecDouble : return add_vector_vector<vQDateTime> (a.as<vQDateTime>(), b.as<vdouble>());
        default              : ;
        }
        break;
    case Type::VecBareDate:
        switch (b.type()) {
        case Type::Int       : return add_vector_scalar<vBareDate> (a.as<vBareDate>(), b.as<int>());
        case Type::Double    : return add_vector_scalar<vBareDate> (a.as<vBareDate>(), b.as<double>());
        case Type::VecInt    : return add_vector_vector<vBareDate> (a.as<vBareDate>(), b.as<vint>());
        case Type::VecDouble : return add_vector_vector<vBareDate> (a.as<vBareDate>(), b.as<vdouble>());
        default              : ;
        }
        break;
    }
    ThrowException("Incompatible operands in addition").value(a.typeName() +" + "+ b.typeName());
    return Value();
}

//
// Subtraction
//

Value subtract(const Value &a, const Value &b) {
    using Type = Value::Type;

    if (!a.isVector() && b.isVector())
        ThrowException("Vector (v) must preceed scalar (s) in subtraction: v-s not s-v")
                .value(a.typeName() +"-"+ (b.typeName()));

    switch (a.type()) {
    case Type::Uninitialized :
        break;
    case Type::Bool:
        break;
    case Type::Int:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<int>() - b.as<int>());
        case Type::Double  : return Value(a.as<int>() - b.as<double>());
        default            : ;
        }
        break;
    case Type::Double:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<double>() - b.as<int>());
        case Type::Double  : return Value(a.as<double>() - b.as<double>());
        default            : ;
        }
        break;
    case Type::String:
        break;
    case Type::Date:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<QDate>() - b.as<int>());
        case Type::Double  : return Value(a.as<QDate>() - b.as<double>());
        case Type::Date    : return Value(a.as<QDate>() - b.as<QDate>());
        default            : ;
        }
        break;
    case Type::Time:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<QTime>() - b.as<int>());
        case Type::Double  : return Value(a.as<QTime>() - b.as<double>());
        case Type::Time    : return Value(a.as<QTime>() - b.as<QTime>());
        default            : ;
        }
        break;
    case Type::DateTime:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<QDateTime>() - b.as<int>());
        case Type::Double  : return Value(a.as<QDateTime>() - b.as<double>());
        case Type::DateTime: return Value(a.as<QDateTime>() - b.as<QDateTime>());
        default            : ;
        }
        break;
    case Type::BareDate:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<BareDate>() - b.as<int>());
        case Type::Double  : return Value(a.as<BareDate>() - b.as<double>());
        case Type::Date    : return Value(a.as<BareDate>() - b.as<BareDate>());
        default            : ;
        }
        break;
    //
    // Vector operations
    //
    case Type::VecBool:
        break;
    case Type::VecInt:
        switch (b.type()) {
        case Type::Int        : return subtract_vector_scalar<vint>    (a.as<vint>(), b.as<int>());
        case Type::Double     : return subtract_vector_scalar<vdouble> (a.as<vint>(), b.as<double>());
        case Type::VecInt     : return subtract_vector_vector<vint>    (a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return subtract_vector_vector<vdouble> (a.as<vint>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        switch (b.type()) {
        case Type::Int        : return subtract_vector_scalar<vdouble> (a.as<vdouble>(), b.as<int>());
        case Type::Double     : return subtract_vector_scalar<vdouble> (a.as<vdouble>(), b.as<double>());
        case Type::VecInt     : return subtract_vector_vector<vdouble> (a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return subtract_vector_vector<vdouble> (a.as<vdouble>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecString:
        break;
    case Type::VecDate:
        switch (b.type()) {
        case Type::Int       : return subtract_vector_scalar<vQDate> (a.as<vQDate>(), b.as<int>());
        case Type::Double    : return subtract_vector_scalar<vQDate> (a.as<vQDate>(), b.as<double>());
        case Type::VecInt    : return subtract_vector_vector<vQDate> (a.as<vQDate>(), b.as<vint>());
        case Type::VecDouble : return subtract_vector_vector<vQDate> (a.as<vQDate>(), b.as<vdouble>());
        default              : ;
        }
        break;
    case Type::VecTime:
        switch (b.type()) {
        case Type::Int       : return subtract_vector_scalar<vQTime> (a.as<vQTime>(), b.as<int>());
        case Type::Double    : return subtract_vector_scalar<vQTime> (a.as<vQTime>(), b.as<double>());
        case Type::VecInt    : return subtract_vector_vector<vQTime> (a.as<vQTime>(), b.as<vint>());
        case Type::VecDouble : return subtract_vector_vector<vQTime> (a.as<vQTime>(), b.as<vdouble>());
        default            : ;
        }
        break;
    case Type::VecDateTime:
        switch (b.type()) {
        case Type::Int       : return subtract_vector_scalar<vQDateTime> (a.as<vQDateTime>(), b.as<int>());
        case Type::Double    : return subtract_vector_scalar<vQDateTime> (a.as<vQDateTime>(), b.as<double>());
        case Type::VecInt    : return subtract_vector_vector<vQDateTime> (a.as<vQDateTime>(), b.as<vint>());
        case Type::VecDouble : return subtract_vector_vector<vQDateTime> (a.as<vQDateTime>(), b.as<vdouble>());
        default              : ;
        }
        break;
    case Type::VecBareDate:
        switch (b.type()) {
        case Type::Int       : return subtract_vector_scalar<vBareDate> (a.as<vBareDate>(), b.as<int>());
        case Type::Double    : return subtract_vector_scalar<vBareDate> (a.as<vBareDate>(), b.as<double>());
        case Type::VecInt    : return subtract_vector_vector<vBareDate> (a.as<vBareDate>(), b.as<vint>());
        case Type::VecDouble : return subtract_vector_vector<vBareDate> (a.as<vBareDate>(), b.as<vdouble>());
        default              : ;
        }
        break;
    }
    ThrowException("Incompatible operands in subtraction").value(a.typeName() +" - "+ b.typeName());
    return Value();
}


//
// Multiplication
//

Value multiply(const Value &a, const Value &b) {
    using Type = Value::Type;

    if (!a.isVector() && b.isVector()) return add(b, a);

    switch (a.type()) {
    case Type::Int:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<int>() * b.as<int>());
        case Type::Double  : return Value(a.as<int>() * b.as<double>());
        default            : ;
        }
        break;
    case Type::Double:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<double>() * b.as<int>());
        case Type::Double  : return Value(a.as<double>() * b.as<double>());
        default            : ;
        }
        break;
    case Type::VecInt:
        switch (b.type()) {
        case Type::Int        : return multiply_vector_scalar<vint>      (a.as<vint>(), b.as<int>());
        case Type::Double     : return multiply_vector_scalar<vdouble>   (a.as<vint>(), b.as<double>());
        case Type::VecInt     : return multiply_vector_vector<vint>      (a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return multiply_vector_vector<vdouble>   (a.as<vint>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        switch (b.type()) {
        case Type::Int        : return multiply_vector_scalar<vdouble>   (a.as<vdouble>(), b.as<int>());
        case Type::Double     : return multiply_vector_scalar<vdouble>   (a.as<vdouble>(), b.as<double>());
        case Type::VecInt     : return multiply_vector_vector<vdouble>   (a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return multiply_vector_vector<vdouble>   (a.as<vdouble>(), b.as<vdouble>());
        default               : ;
        }
        break;
    default: ;
    }
    ThrowException("Incompatible operands in multiplication").value(a.typeName() +" * "+ b.typeName());
    return Value();
}

//
// Division
//

Value divide(const Value &a, const Value &b) {
    using Type = Value::Type;

    if (!a.isVector() && b.isVector())
        ThrowException("Vector (v) must preceed scalar (s) in division: v/s not s/v")
            .value(a.typeName() +"/"+ (b.typeName()));

    switch (a.type()) {
    case Type::Int:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<int>() / b.as<int>());
        case Type::Double  : return Value(a.as<int>() / b.as<double>());
        default            : ;
        }
        break;
    case Type::Double:
        switch (b.type()) {
        case Type::Int     : return Value(a.as<double>() / b.as<int>());
        case Type::Double  : return Value(a.as<double>() / b.as<double>());
        default            : ;
        }
        break;
    case Type::VecInt:
        switch (b.type()) {
        case Type::Int        : return divide_vector_scalar<vint>      (a.as<vint>(), b.as<int>());
        case Type::Double     : return divide_vector_scalar<vdouble>   (a.as<vint>(), b.as<double>());
        case Type::VecInt     : return divide_vector_vector<vint>      (a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return divide_vector_vector<vdouble>   (a.as<vint>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        switch (b.type()) {
        case Type::Int        : return divide_vector_scalar<vdouble>   (a.as<vdouble>(), b.as<int>());
        case Type::Double     : return divide_vector_scalar<vdouble>   (a.as<vdouble>(), b.as<double>());
        case Type::VecInt     : return divide_vector_vector<vdouble>   (a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return divide_vector_vector<vdouble>   (a.as<vdouble>(), b.as<vdouble>());
        default               : ;
        }
        break;
    default: ;
    }
    ThrowException("Incompatible operands in division").value(a.typeName() +" / "+ b.typeName());
    return Value();
}

//
// Exponentiation
//

Value exponentiate(const Value &a, const Value &b) {
    using Type = Value::Type;
    using std::pow;

    if (!a.isVector() && b.isVector())
        ThrowException("Vector (v) must preceed scalar (s) in exponentiation: v^s not s^v")
            .value(a.typeName() +"/"+ (b.typeName()));

    switch (a.type()) {
    case Type::Int:
        switch (b.type()) {
        case Type::Int     : return Value(ipow(a.as<int>(), b.as<int>()));
        case Type::Double  : return Value( pow(a.as<int>(), b.as<double>()));
        default            : ;
        }
        break;
    case Type::Double:
        switch (b.type()) {
        case Type::Int     : return Value(pow(a.as<double>(), b.as<int>()));
        case Type::Double  : return Value(pow(a.as<double>(), b.as<double>()));
        default            : ;
        }
        break;
    case Type::VecInt:
        switch (b.type()) {
        case Type::Int        : return exp_vector_scalar<vint>    (a.as<vint>(), b.as<int>());
        case Type::Double     : return exp_vector_scalar<vdouble> (a.as<vint>(), b.as<double>());
        case Type::VecInt     : return exp_vector_vector<vint>    (a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return exp_vector_vector<vdouble> (a.as<vint>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        switch (b.type()) {
        case Type::Int        : return exp_vector_scalar<vdouble> (a.as<vdouble>(), b.as<int>());
        case Type::Double     : return exp_vector_scalar<vdouble> (a.as<vdouble>(), b.as<double>());
        case Type::VecInt     : return exp_vector_vector<vdouble> (a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return exp_vector_vector<vdouble> (a.as<vdouble>(), b.as<vdouble>());
        default               : ;
        }
        break;
    default: ;
    }
    ThrowException("Incompatible operands in division").value(a.typeName() +" / "+ b.typeName());
    return Value();
}

//
// Logical and
//

Value and_(const Value &a, const Value &b) {
    using Type = Value::Type;

    switch (a.type()) {
    case Type::Bool:
        switch (b.type()) {
        case Type::Bool     : return a.as<bool>() && b.as<bool>();
        case Type::VecBool  : return and_vector_scalar(b.as<vbool>(), b.as<bool>());
        default            : ;
        }
        break;
    case Type::VecBool:
        switch (b.type()) {
        case Type::Bool     : return and_vector_scalar(b.as<vbool>(), a.as< bool>());
        case Type::VecBool  : return and_vector_vector(a.as<vbool>(), b.as<vbool>());
        default            : ;
        }
        break;
    default: ;
    }
    ThrowException("Incompatible operands for logical 'and'").value(a.typeName() +" && "+ b.typeName());
    return Value();
}

//
// Logical and
//

Value or_(const Value &a, const Value &b) {
    using Type = Value::Type;

    switch (a.type()) {
    case Type::Bool:
        switch (b.type()) {
        case Type::Bool     : return a.as<bool>() || b.as<bool>();
        case Type::VecBool  : return or_vector_scalar(b.as<vbool>(), b.as<bool>());
        default            : ;
        }
        break;
    case Type::VecBool:
        switch (b.type()) {
        case Type::Bool     : return or_vector_scalar(b.as<vbool>(), a.as< bool>());
        case Type::VecBool  : return or_vector_vector(a.as<vbool>(), b.as<vbool>());
        default            : ;
        }
        break;
    default: ;
    }
    ThrowException("Incompatible operands for logical 'or'").value(a.typeName() +" || "+ b.typeName());
    return Value();
}

//
// Logical not
//

Value not_(const Value &a) {
    using Type = Value::Type;

    switch (a.type()) {
    case Type::Bool:
        return Value(!a.as<bool>());
    case Type::VecBool:
        return Value(not_vector(a.as<vbool>()));
    default: ;
    }
    ThrowException("Incompatible operand for logical 'not'").value("! " + a.typeName());
    return Value();
}

//
// Negation
//

Value negate(const Value &a) {
    using Type = Value::Type;

    switch (a.type()) {
    case Type::Int:
        return Value(-a.as<int>());
    case Type::Double:
        return Value(-a.as<double>());
    case Type::VecInt:
        return negate_vector(a.as<vint>());
    case Type::VecDouble:
        return negate_vector(a.as<vdouble>());
    default: ;
    }
    ThrowException("Operand cannot be negated").value("- " + a.typeName());
    return Value();
}

}

