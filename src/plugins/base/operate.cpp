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
        case Type::Int     : return Value(a.value<int>() + b.value<int>());
        case Type::Double  : return Value(a.value<int>() + b.value<double>());
        case Type::Date    : return Value(a.value<int>() + b.value<QDate>());
        case Type::Time    : return Value(a.value<int>() + b.value<QTime>());
        case Type::DateTime: return Value(a.value<int>() + b.value<QDateTime>());
        case Type::BareDate: return Value(a.value<int>() + b.value<BareDate>());
        default            : ;
        }
        break;
    case Type::Double:
        switch (b.type()) {
        case Type::Int     : return Value(a.value<double>() + b.value<int>());
        case Type::Double  : return Value(a.value<double>() + b.value<double>());
        case Type::Date    : return Value(a.value<double>() + b.value<QDate>());
        case Type::Time    : return Value(a.value<double>() + b.value<QTime>());
        case Type::DateTime: return Value(a.value<double>() + b.value<QDateTime>());
        case Type::BareDate: return Value(a.value<double>() + b.value<BareDate>());
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
        case Type::Int     : return Value(a.value<QDate>() + b.value<int>());
        case Type::Double  : return Value(a.value<QDate>() + b.value<double>());
        default            : ;
        }
        break;
    case Type::Time:
        switch (b.type()) {
        case Type::Int     : return Value(a.value<QTime>() + b.value<int>());
        case Type::Double  : return Value(a.value<QTime>() + b.value<double>());
        default            : ;
        }
        break;
    case Type::DateTime:
        switch (b.type()) {
        case Type::Int     : return Value(a.value<QDateTime>() + b.value<int>());
        case Type::Double  : return Value(a.value<QDateTime>() + b.value<double>());
        default            : ;
        }
        break;
    case Type::BareDate:
        switch (b.type()) {
        case Type::Int     : return Value(a.value<BareDate>() + b.value<int>());
        case Type::Double  : return Value(a.value<BareDate>() + b.value<double>());
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
        case Type::Int     : return add_vector_scalar<vint>      (a.value<vint>(), b.value<int>());
        case Type::Double  : return add_vector_scalar<vdouble>   (a.value<vint>(), b.value<double>());
        case Type::Date    : return add_vector_scalar<vQDate>    (a.value<vint>(), b.value<QDate>());
        case Type::Time    : return add_vector_scalar<vQTime>    (a.value<vint>(), b.value<QTime>());
        case Type::DateTime: return add_vector_scalar<vQDateTime>(a.value<vint>(), b.value<QDateTime>());
        case Type::BareDate: return add_vector_scalar<vBareDate> (a.value<vint>(), b.value<BareDate>());

        case Type::VecInt     : return add_vector_vector<vint>      (a.value<vint>(), b.value<vint>());
        case Type::VecDouble  : return add_vector_vector<vdouble>   (a.value<vint>(), b.value<vdouble>());
        case Type::VecDate    : return add_vector_vector<vQDate>    (a.value<vint>(), b.value<vQDate>());
        case Type::VecTime    : return add_vector_vector<vQTime>    (a.value<vint>(), b.value<vQTime>());
        case Type::VecDateTime: return add_vector_vector<vQDateTime>(a.value<vint>(), b.value<vQDateTime>());
        case Type::VecBareDate: return add_vector_vector<vBareDate> (a.value<vint>(), b.value<vBareDate>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        switch (b.type()) {
        case Type::Int     : return add_vector_scalar<vdouble>   (a.value<vdouble>(), b.value<int>());
        case Type::Double  : return add_vector_scalar<vdouble>   (a.value<vdouble>(), b.value<double>());
        case Type::Date    : return add_vector_scalar<vQDate>    (a.value<vdouble>(), b.value<QDate>());
        case Type::Time    : return add_vector_scalar<vQTime>    (a.value<vdouble>(), b.value<QTime>());
        case Type::DateTime: return add_vector_scalar<vQDateTime>(a.value<vdouble>(), b.value<QDateTime>());
        case Type::BareDate: return add_vector_scalar<vBareDate> (a.value<vdouble>(), b.value<BareDate>());

        case Type::VecInt     : return add_vector_vector<vdouble>   (a.value<vdouble>(), b.value<vint>());
        case Type::VecDouble  : return add_vector_vector<vdouble>   (a.value<vdouble>(), b.value<vdouble>());
        case Type::VecDate    : return add_vector_vector<vQDate>    (a.value<vdouble>(), b.value<vQDate>());
        case Type::VecTime    : return add_vector_vector<vQTime>    (a.value<vdouble>(), b.value<vQTime>());
        case Type::VecDateTime: return add_vector_vector<vQDateTime>(a.value<vdouble>(), b.value<vQDateTime>());
        case Type::VecBareDate: return add_vector_vector<vBareDate> (a.value<vdouble>(), b.value<vBareDate>());
        default               : ;
        }
        break;
    case Type::VecString:
        switch (b.type()) {
        case Type::String    : return add_vector_scalar<vQString> (a.value<vQString>(), b.value< QString>());
        case Type::VecString : return add_vector_vector<vQString> (a.value<vQString>(), b.value<vQString>());
        default              : ;
        }
        break;
    case Type::VecDate:
        switch (b.type()) {
        case Type::Int       : return add_vector_scalar<vQDate> (a.value<vQDate>(), b.value<int>());
        case Type::Double    : return add_vector_scalar<vQDate> (a.value<vQDate>(), b.value<double>());
        case Type::VecInt    : return add_vector_vector<vQDate> (a.value<vQDate>(), b.value<vint>());
        case Type::VecDouble : return add_vector_vector<vQDate> (a.value<vQDate>(), b.value<vdouble>());
        default              : ;
        }
        break;
    case Type::VecTime:
        switch (b.type()) {
        case Type::Int       : return add_vector_scalar<vQTime> (a.value<vQTime>(), b.value<int>());
        case Type::Double    : return add_vector_scalar<vQTime> (a.value<vQTime>(), b.value<double>());
        case Type::VecInt    : return add_vector_vector<vQTime> (a.value<vQTime>(), b.value<vint>());
        case Type::VecDouble : return add_vector_vector<vQTime> (a.value<vQTime>(), b.value<vdouble>());
        default            : ;
        }
        break;
    case Type::VecDateTime:
        switch (b.type()) {
        case Type::Int       : return add_vector_scalar<vQDateTime> (a.value<vQDateTime>(), b.value<int>());
        case Type::Double    : return add_vector_scalar<vQDateTime> (a.value<vQDateTime>(), b.value<double>());
        case Type::VecInt    : return add_vector_vector<vQDateTime> (a.value<vQDateTime>(), b.value<vint>());
        case Type::VecDouble : return add_vector_vector<vQDateTime> (a.value<vQDateTime>(), b.value<vdouble>());
        default              : ;
        }
        break;
    case Type::VecBareDate:
        switch (b.type()) {
        case Type::Int       : return add_vector_scalar<vBareDate> (a.value<vBareDate>(), b.value<int>());
        case Type::Double    : return add_vector_scalar<vBareDate> (a.value<vBareDate>(), b.value<double>());
        case Type::VecInt    : return add_vector_vector<vBareDate> (a.value<vBareDate>(), b.value<vint>());
        case Type::VecDouble : return add_vector_vector<vBareDate> (a.value<vBareDate>(), b.value<vdouble>());
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
        case Type::Int     : return Value(a.value<int>() - b.value<int>());
        case Type::Double  : return Value(a.value<int>() - b.value<double>());
        default            : ;
        }
        break;
    case Type::Double:
        switch (b.type()) {
        case Type::Int     : return Value(a.value<double>() - b.value<int>());
        case Type::Double  : return Value(a.value<double>() - b.value<double>());
        default            : ;
        }
        break;
    case Type::String:
        break;
    case Type::Date:
        switch (b.type()) {
        case Type::Int     : return Value(a.value<QDate>() - b.value<int>());
        case Type::Double  : return Value(a.value<QDate>() - b.value<double>());
        case Type::Date    : return Value(a.value<QDate>() - b.value<QDate>());
        default            : ;
        }
        break;
    case Type::Time:
        switch (b.type()) {
        case Type::Int     : return Value(a.value<QTime>() - b.value<int>());
        case Type::Double  : return Value(a.value<QTime>() - b.value<double>());
        case Type::Time    : return Value(a.value<QTime>() - b.value<QTime>());
        default            : ;
        }
        break;
    case Type::DateTime:
        switch (b.type()) {
        case Type::Int     : return Value(a.value<QDateTime>() - b.value<int>());
        case Type::Double  : return Value(a.value<QDateTime>() - b.value<double>());
        case Type::DateTime: return Value(a.value<QDateTime>() - b.value<QDateTime>());
        default            : ;
        }
        break;
    case Type::BareDate:
        switch (b.type()) {
        case Type::Int     : return Value(a.value<BareDate>() - b.value<int>());
        case Type::Double  : return Value(a.value<BareDate>() - b.value<double>());
        case Type::Date    : return Value(a.value<BareDate>() - b.value<BareDate>());
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
        case Type::Int        : return subtract_vector_scalar<vint>    (a.value<vint>(), b.value<int>());
        case Type::Double     : return subtract_vector_scalar<vdouble> (a.value<vint>(), b.value<double>());
        case Type::VecInt     : return subtract_vector_vector<vint>    (a.value<vint>(), b.value<vint>());
        case Type::VecDouble  : return subtract_vector_vector<vdouble> (a.value<vint>(), b.value<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        switch (b.type()) {
        case Type::Int        : return subtract_vector_scalar<vdouble> (a.value<vdouble>(), b.value<int>());
        case Type::Double     : return subtract_vector_scalar<vdouble> (a.value<vdouble>(), b.value<double>());
        case Type::VecInt     : return subtract_vector_vector<vdouble> (a.value<vdouble>(), b.value<vint>());
        case Type::VecDouble  : return subtract_vector_vector<vdouble> (a.value<vdouble>(), b.value<vdouble>());
        default               : ;
        }
        break;
    case Type::VecString:
        break;
    case Type::VecDate:
        switch (b.type()) {
        case Type::Int       : return subtract_vector_scalar<vQDate> (a.value<vQDate>(), b.value<int>());
        case Type::Double    : return subtract_vector_scalar<vQDate> (a.value<vQDate>(), b.value<double>());
        case Type::VecInt    : return subtract_vector_vector<vQDate> (a.value<vQDate>(), b.value<vint>());
        case Type::VecDouble : return subtract_vector_vector<vQDate> (a.value<vQDate>(), b.value<vdouble>());
        default              : ;
        }
        break;
    case Type::VecTime:
        switch (b.type()) {
        case Type::Int       : return subtract_vector_scalar<vQTime> (a.value<vQTime>(), b.value<int>());
        case Type::Double    : return subtract_vector_scalar<vQTime> (a.value<vQTime>(), b.value<double>());
        case Type::VecInt    : return subtract_vector_vector<vQTime> (a.value<vQTime>(), b.value<vint>());
        case Type::VecDouble : return subtract_vector_vector<vQTime> (a.value<vQTime>(), b.value<vdouble>());
        default            : ;
        }
        break;
    case Type::VecDateTime:
        switch (b.type()) {
        case Type::Int       : return subtract_vector_scalar<vQDateTime> (a.value<vQDateTime>(), b.value<int>());
        case Type::Double    : return subtract_vector_scalar<vQDateTime> (a.value<vQDateTime>(), b.value<double>());
        case Type::VecInt    : return subtract_vector_vector<vQDateTime> (a.value<vQDateTime>(), b.value<vint>());
        case Type::VecDouble : return subtract_vector_vector<vQDateTime> (a.value<vQDateTime>(), b.value<vdouble>());
        default              : ;
        }
        break;
    case Type::VecBareDate:
        switch (b.type()) {
        case Type::Int       : return subtract_vector_scalar<vBareDate> (a.value<vBareDate>(), b.value<int>());
        case Type::Double    : return subtract_vector_scalar<vBareDate> (a.value<vBareDate>(), b.value<double>());
        case Type::VecInt    : return subtract_vector_vector<vBareDate> (a.value<vBareDate>(), b.value<vint>());
        case Type::VecDouble : return subtract_vector_vector<vBareDate> (a.value<vBareDate>(), b.value<vdouble>());
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
        case Type::Int     : return Value(a.value<int>() * b.value<int>());
        case Type::Double  : return Value(a.value<int>() * b.value<double>());
        default            : ;
        }
        break;
    case Type::Double:
        switch (b.type()) {
        case Type::Int     : return Value(a.value<double>() * b.value<int>());
        case Type::Double  : return Value(a.value<double>() * b.value<double>());
        default            : ;
        }
        break;
    case Type::VecInt:
        switch (b.type()) {
        case Type::Int        : return multiply_vector_scalar<vint>      (a.value<vint>(), b.value<int>());
        case Type::Double     : return multiply_vector_scalar<vdouble>   (a.value<vint>(), b.value<double>());
        case Type::VecInt     : return multiply_vector_vector<vint>      (a.value<vint>(), b.value<vint>());
        case Type::VecDouble  : return multiply_vector_vector<vdouble>   (a.value<vint>(), b.value<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        switch (b.type()) {
        case Type::Int        : return multiply_vector_scalar<vdouble>   (a.value<vdouble>(), b.value<int>());
        case Type::Double     : return multiply_vector_scalar<vdouble>   (a.value<vdouble>(), b.value<double>());
        case Type::VecInt     : return multiply_vector_vector<vdouble>   (a.value<vdouble>(), b.value<vint>());
        case Type::VecDouble  : return multiply_vector_vector<vdouble>   (a.value<vdouble>(), b.value<vdouble>());
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
        case Type::Int     : return Value(a.value<int>() / b.value<int>());
        case Type::Double  : return Value(a.value<int>() / b.value<double>());
        default            : ;
        }
        break;
    case Type::Double:
        switch (b.type()) {
        case Type::Int     : return Value(a.value<double>() / b.value<int>());
        case Type::Double  : return Value(a.value<double>() / b.value<double>());
        default            : ;
        }
        break;
    case Type::VecInt:
        switch (b.type()) {
        case Type::Int        : return divide_vector_scalar<vint>      (a.value<vint>(), b.value<int>());
        case Type::Double     : return divide_vector_scalar<vdouble>   (a.value<vint>(), b.value<double>());
        case Type::VecInt     : return divide_vector_vector<vint>      (a.value<vint>(), b.value<vint>());
        case Type::VecDouble  : return divide_vector_vector<vdouble>   (a.value<vint>(), b.value<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        switch (b.type()) {
        case Type::Int        : return divide_vector_scalar<vdouble>   (a.value<vdouble>(), b.value<int>());
        case Type::Double     : return divide_vector_scalar<vdouble>   (a.value<vdouble>(), b.value<double>());
        case Type::VecInt     : return divide_vector_vector<vdouble>   (a.value<vdouble>(), b.value<vint>());
        case Type::VecDouble  : return divide_vector_vector<vdouble>   (a.value<vdouble>(), b.value<vdouble>());
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
        case Type::Int     : return Value(ipow(a.value<int>(), b.value<int>()));
        case Type::Double  : return Value( pow(a.value<int>(), b.value<double>()));
        default            : ;
        }
        break;
    case Type::Double:
        switch (b.type()) {
        case Type::Int     : return Value(pow(a.value<double>(), b.value<int>()));
        case Type::Double  : return Value(pow(a.value<double>(), b.value<double>()));
        default            : ;
        }
        break;
    case Type::VecInt:
        switch (b.type()) {
        case Type::Int        : return exp_vector_scalar<vint>    (a.value<vint>(), b.value<int>());
        case Type::Double     : return exp_vector_scalar<vdouble> (a.value<vint>(), b.value<double>());
        case Type::VecInt     : return exp_vector_vector<vint>    (a.value<vint>(), b.value<vint>());
        case Type::VecDouble  : return exp_vector_vector<vdouble> (a.value<vint>(), b.value<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        switch (b.type()) {
        case Type::Int        : return exp_vector_scalar<vdouble> (a.value<vdouble>(), b.value<int>());
        case Type::Double     : return exp_vector_scalar<vdouble> (a.value<vdouble>(), b.value<double>());
        case Type::VecInt     : return exp_vector_vector<vdouble> (a.value<vdouble>(), b.value<vint>());
        case Type::VecDouble  : return exp_vector_vector<vdouble> (a.value<vdouble>(), b.value<vdouble>());
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
        case Type::Bool     : return a.value<bool>() && b.value<bool>();
        case Type::VecBool  : return and_vector_scalar(b.value<vbool>(), b.value<bool>());
        default            : ;
        }
        break;
    case Type::VecBool:
        switch (b.type()) {
        case Type::Bool     : return and_vector_scalar(b.value<vbool>(), a.value< bool>());
        case Type::VecBool  : return and_vector_vector(a.value<vbool>(), b.value<vbool>());
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
        case Type::Bool     : return a.value<bool>() || b.value<bool>();
        case Type::VecBool  : return or_vector_scalar(b.value<vbool>(), b.value<bool>());
        default            : ;
        }
        break;
    case Type::VecBool:
        switch (b.type()) {
        case Type::Bool     : return and_vector_scalar(b.value<vbool>(), a.value< bool>());
        case Type::VecBool  : return or_vector_vector(a.value<vbool>(), b.value<vbool>());
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
        return Value(!a.value<bool>());
    case Type::VecBool:
        return Value(not_vector(a.value<vbool>()));
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
        return Value(-a.value<int>());
    case Type::Double:
        return Value(-a.value<double>());
    case Type::VecInt:
        return negate_vector(a.value<vint>());
    case Type::VecDouble:
        return negate_vector(a.value<vdouble>());
    default: ;
    }
    ThrowException("Operand cannot be negated").value("- " + a.typeName());
    return Value();
}

}

