/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QDate>
#include <QDateTime>
#include <QTime>
#include "port_value_op.h"
#include "exception.h"

using namespace base;

namespace port_value_op {

//
// Conversions
//
#define PORT_VALUE_UNARY(T) \
T *v2 = reinterpret_cast<T*>(v); \
const T *x2 = reinterpret_cast<const T*>(x)

#define PORT_VALUE_BINARY(T) \
T *v2 = reinterpret_cast<T*>(v); \
const T *x2 = reinterpret_cast<const T*>(x); \
const T *y2 = reinterpret_cast<const T*>(y)

#define PORT_VALUE_COMPARE(T) \
const T *x2 = reinterpret_cast<const T*>(x); \
const T *y2 = reinterpret_cast<const T*>(y)

//
// Creational
//
#define CASE_PORT_VALUE_ALLOCATE(Tcpp, Tport) \
case Tport: return new Tcpp; break

void* allocate(base::PortType type) {
    switch (type) {
    CASE_PORT_VALUE_ALLOCATE(bool, Bool);
    CASE_PORT_VALUE_ALLOCATE(char, Char);
    CASE_PORT_VALUE_ALLOCATE(int, Int);
    CASE_PORT_VALUE_ALLOCATE(long int, LongInt);
    CASE_PORT_VALUE_ALLOCATE(long long int, LongLongInt);
    CASE_PORT_VALUE_ALLOCATE(float, Float);
    CASE_PORT_VALUE_ALLOCATE(double, Double);
    CASE_PORT_VALUE_ALLOCATE(long double, LongDouble);
    CASE_PORT_VALUE_ALLOCATE(QString, String);
    CASE_PORT_VALUE_ALLOCATE(QDate, Date);
    CASE_PORT_VALUE_ALLOCATE(QTime, Time);
    CASE_PORT_VALUE_ALLOCATE(QDateTime, DateTime);
    default:
        ThrowException("Cannot allocate port value of this type").value(nameOf(type));
    }
    return 0;
}

#define CASE_PORT_VALUE_DEALLOCATE(Tcpp, Tport) \
case Tport: delete reinterpret_cast<Tcpp*>(v); v=0; break

void deallocate(base::PortType type, void *v) {
    if (!v) return;
    switch (type) {
    CASE_PORT_VALUE_DEALLOCATE(bool, Bool);
    CASE_PORT_VALUE_DEALLOCATE(char, Char);
    CASE_PORT_VALUE_DEALLOCATE(int, Int);
    CASE_PORT_VALUE_DEALLOCATE(long int, LongInt);
    CASE_PORT_VALUE_DEALLOCATE(long long int, LongLongInt);
    CASE_PORT_VALUE_DEALLOCATE(float, Float);
    CASE_PORT_VALUE_DEALLOCATE(double, Double);
    CASE_PORT_VALUE_DEALLOCATE(long double, LongDouble);
    CASE_PORT_VALUE_DEALLOCATE(QString, String);
    CASE_PORT_VALUE_DEALLOCATE(QDate, Date);
    CASE_PORT_VALUE_DEALLOCATE(QTime, Time);
    CASE_PORT_VALUE_DEALLOCATE(QDateTime, DateTime);
    default:
        ThrowException("Cannot deallocate port value of this type").value(nameOf(type));
    }
}

//
// Unary
//

#define DECLARE_PORT_VALUE_ACCUMULATE(Tcpp, Tport) \
void accumulate##Tport(void *v, const void *x) { \
    PORT_VALUE_UNARY(Tcpp); \
    *v2 += *x2; \
}
DECLARE_PORT_VALUE_ACCUMULATE(int, Int)
DECLARE_PORT_VALUE_ACCUMULATE(long int, LongInt)
DECLARE_PORT_VALUE_ACCUMULATE(long long int, LongLongInt)
DECLARE_PORT_VALUE_ACCUMULATE(float, Float)
DECLARE_PORT_VALUE_ACCUMULATE(double, Double)
DECLARE_PORT_VALUE_ACCUMULATE(long double, LongDouble)

#define CASE_PORT_VALUE_ACCUMULATE(Tcpp, Tport) \
case Tport: accumulate##Tport(v, x); break

void accumulate(base::PortType type, void *v, const void *x) {
    switch (type) {
    CASE_PORT_VALUE_ACCUMULATE(int, Int);
    CASE_PORT_VALUE_ACCUMULATE(long int, LongInt);
    CASE_PORT_VALUE_ACCUMULATE(long long int, LongLongInt);
    CASE_PORT_VALUE_ACCUMULATE(float, Float);
    CASE_PORT_VALUE_ACCUMULATE(double, Double);
    CASE_PORT_VALUE_ACCUMULATE(long double, LongDouble);
    default:
        ThrowException("Cannot accumulate Port value of this type").value(nameOf(type));
    }
}

#define DECLARE_PORT_VALUE_DIVIDE(Tcpp, Tport) \
void divide##Tport(void *v, double x) { \
    Tcpp *v2 = reinterpret_cast<Tcpp*>(v); \
    *v2 /= x; \
}
DECLARE_PORT_VALUE_DIVIDE(int, Int)
DECLARE_PORT_VALUE_DIVIDE(long int, LongInt)
DECLARE_PORT_VALUE_DIVIDE(long long int, LongLongInt)
DECLARE_PORT_VALUE_DIVIDE(float, Float)
DECLARE_PORT_VALUE_DIVIDE(double, Double)
DECLARE_PORT_VALUE_DIVIDE(long double, LongDouble)

#define CASE_PORT_VALUE_DIVIDE(Tcpp, Tport) \
case Tport: divide##Tport(v, x); break

void divide(base::PortType type, void *v, double x) {
    switch (type) {
    CASE_PORT_VALUE_DIVIDE(int, Int);
    CASE_PORT_VALUE_DIVIDE(long int, LongInt);
    CASE_PORT_VALUE_DIVIDE(long long int, LongLongInt);
    CASE_PORT_VALUE_DIVIDE(float, Float);
    CASE_PORT_VALUE_DIVIDE(double, Double);
    CASE_PORT_VALUE_DIVIDE(long double, LongDouble);
    default:
        ThrowException("Cannot divide Port value of this type").value(nameOf(type));
    }
}

//
// Binary
//

#define DECLARE_PORT_VALUE_ADD(Tcpp, Tport) \
void add##Tport(void *v, const void *x, const void *y) { \
    PORT_VALUE_BINARY(Tcpp); \
    *v2 = *x2 + *y2; \
}
DECLARE_PORT_VALUE_ADD(int, Int)
DECLARE_PORT_VALUE_ADD(long int, LongInt)
DECLARE_PORT_VALUE_ADD(long long int, LongLongInt)
DECLARE_PORT_VALUE_ADD(float, Float)
DECLARE_PORT_VALUE_ADD(double, Double)
DECLARE_PORT_VALUE_ADD(long double, LongDouble)

#define CASE_PORT_VALUE_ADD(Tcpp, Tport) \
case Tport: add##Tport(v, x, y); break

void add(base::PortType type, void *v, const void *x, const void *y) {
    switch (type) {
    CASE_PORT_VALUE_ADD(int, Int);
    CASE_PORT_VALUE_ADD(long int, LongInt);
    CASE_PORT_VALUE_ADD(long long int, LongLongInt);
    CASE_PORT_VALUE_ADD(float, Float);
    CASE_PORT_VALUE_ADD(double, Double);
    CASE_PORT_VALUE_ADD(long double, LongDouble);
    default:
        ThrowException("Cannot add Port values of this type").value(nameOf(type));
    }
}

#define DECLARE_PORT_VALUE_MIN(Tcpp, Tport) \
void min##Tport(void *v, const void *x, const void *y) { \
    PORT_VALUE_BINARY(Tcpp); \
    *v2 = (*x2 < *y2) ? *x2 : *y2; \
}
DECLARE_PORT_VALUE_MIN(bool, Bool)
DECLARE_PORT_VALUE_MIN(int, Int)
DECLARE_PORT_VALUE_MIN(long int, LongInt)
DECLARE_PORT_VALUE_MIN(long long int, LongLongInt)
DECLARE_PORT_VALUE_MIN(float, Float)
DECLARE_PORT_VALUE_MIN(double, Double)
DECLARE_PORT_VALUE_MIN(long double, LongDouble)
DECLARE_PORT_VALUE_MIN(QTime, Time)
DECLARE_PORT_VALUE_MIN(QDate, Date)
DECLARE_PORT_VALUE_MIN(QDateTime, DateTime)

#define CASE_PORT_VALUE_MIN(Tcpp, Tport) \
case Tport: min##Tport(v, x, y); break

void min(base::PortType type, void *v, const void *x, const void *y) {
    switch (type) {
    CASE_PORT_VALUE_MIN(bool, Bool);
    CASE_PORT_VALUE_MIN(int, Int);
    CASE_PORT_VALUE_MIN(long int, LongInt);
    CASE_PORT_VALUE_MIN(long long int, LongLongInt);
    CASE_PORT_VALUE_MIN(float, Float);
    CASE_PORT_VALUE_MIN(double, Double);
    CASE_PORT_VALUE_MIN(long double, LongDouble);
    CASE_PORT_VALUE_MIN(QTime, Time);
    CASE_PORT_VALUE_MIN(QDate, Date);
    CASE_PORT_VALUE_MIN(QDateTime, DateTime);
    default:
        ThrowException("Cannot find minimum of Port values of this type").value(nameOf(type));
    }
}

#define DECLARE_PORT_VALUE_MAX(Tcpp, Tport) \
void max##Tport(void *v, const void *x, const void *y) { \
    PORT_VALUE_BINARY(Tcpp); \
    *v2 = (*x2 > *y2) ? *x2 : *y2; \
}
DECLARE_PORT_VALUE_MAX(bool, Bool)
DECLARE_PORT_VALUE_MAX(int, Int)
DECLARE_PORT_VALUE_MAX(long int, LongInt)
DECLARE_PORT_VALUE_MAX(long long int, LongLongInt)
DECLARE_PORT_VALUE_MAX(float, Float)
DECLARE_PORT_VALUE_MAX(double, Double)
DECLARE_PORT_VALUE_MAX(long double, LongDouble)
DECLARE_PORT_VALUE_MAX(QTime, Time)
DECLARE_PORT_VALUE_MAX(QDate, Date)
DECLARE_PORT_VALUE_MAX(QDateTime, DateTime)

#define CASE_PORT_VALUE_MAX(Tcpp, Tport) \
case Tport: max##Tport(v, x, y); break

void max(base::PortType type, void *v, const void *x, const void *y) {
    switch (type) {
    CASE_PORT_VALUE_MAX(bool, Bool);
    CASE_PORT_VALUE_MAX(int, Int);
    CASE_PORT_VALUE_MAX(long int, LongInt);
    CASE_PORT_VALUE_MAX(long long int, LongLongInt);
    CASE_PORT_VALUE_MAX(float, Float);
    CASE_PORT_VALUE_MAX(double, Double);
    CASE_PORT_VALUE_MAX(long double, LongDouble);
    CASE_PORT_VALUE_MAX(QTime, Time);
    CASE_PORT_VALUE_MAX(QDate, Date);
    CASE_PORT_VALUE_MAX(QDateTime, DateTime);
    default:
        ThrowException("Cannot find maximum of Port values of this type").value(nameOf(type));
    }
}

#define DECLARE_PORT_VALUE_COMPARE(Tcpp, Tport) \
bool lt##Tport(const void *x, const void *y) { \
    PORT_VALUE_COMPARE(Tcpp); \
    return *x2 < *y2; \
} \
bool gt##Tport(const void *x, const void *y) { \
    PORT_VALUE_COMPARE(Tcpp); \
    return *x2 > *y2; \
} \
bool eq##Tport(const void *x, const void *y) { \
    PORT_VALUE_COMPARE(Tcpp); \
    return *x2 == *y2; \
}

DECLARE_PORT_VALUE_COMPARE(bool, Bool)
DECLARE_PORT_VALUE_COMPARE(int, Int)
DECLARE_PORT_VALUE_COMPARE(long int, LongInt)
DECLARE_PORT_VALUE_COMPARE(long long int, LongLongInt)
DECLARE_PORT_VALUE_COMPARE(float, Float)
DECLARE_PORT_VALUE_COMPARE(double, Double)
DECLARE_PORT_VALUE_COMPARE(long double, LongDouble)
DECLARE_PORT_VALUE_COMPARE(QTime, Time)
DECLARE_PORT_VALUE_COMPARE(QDate, Date)
DECLARE_PORT_VALUE_COMPARE(QDateTime, DateTime)

#define CASE_PORT_VALUE_LT(Tcpp, Tport) \
case Tport: return lt##Tport(x, y); break

bool lt(base::PortType type, const void *x, const void *y) {
    switch (type) {
    CASE_PORT_VALUE_LT(bool, Bool);
    CASE_PORT_VALUE_LT(int, Int);
    CASE_PORT_VALUE_LT(long int, LongInt);
    CASE_PORT_VALUE_LT(long long int, LongLongInt);
    CASE_PORT_VALUE_LT(float, Float);
    CASE_PORT_VALUE_LT(double, Double);
    CASE_PORT_VALUE_LT(long double, LongDouble);
    CASE_PORT_VALUE_LT(QTime, Time);
    CASE_PORT_VALUE_LT(QDate, Date);
    CASE_PORT_VALUE_LT(QDateTime, DateTime);
    default:
        ThrowException("Cannot compare Port values of this type").value(nameOf(type));
    }
    return false;
}

#define CASE_PORT_VALUE_GT(Tcpp, Tport) \
case Tport: return gt##Tport(x, y); break

bool gt(base::PortType type, const void *x, const void *y) {
    switch (type) {
    CASE_PORT_VALUE_GT(bool, Bool);
    CASE_PORT_VALUE_GT(int, Int);
    CASE_PORT_VALUE_GT(long int, LongInt);
    CASE_PORT_VALUE_GT(long long int, LongLongInt);
    CASE_PORT_VALUE_GT(float, Float);
    CASE_PORT_VALUE_GT(double, Double);
    CASE_PORT_VALUE_GT(long double, LongDouble);
    CASE_PORT_VALUE_GT(QTime, Time);
    CASE_PORT_VALUE_GT(QDate, Date);
    CASE_PORT_VALUE_GT(QDateTime, DateTime);
    default:
        ThrowException("Cannot compare Port values of this type").value(nameOf(type));
    }
    return false;
}

#define CASE_PORT_VALUE_EQ(Tcpp, Tport) \
case Tport: return eq##Tport(x, y); break

bool eq(base::PortType type, const void *x, const void *y) {
    switch (type) {
    CASE_PORT_VALUE_EQ(bool, Bool);
    CASE_PORT_VALUE_EQ(int, Int);
    CASE_PORT_VALUE_EQ(long int, LongInt);
    CASE_PORT_VALUE_EQ(long long int, LongLongInt);
    CASE_PORT_VALUE_EQ(float, Float);
    CASE_PORT_VALUE_EQ(double, Double);
    CASE_PORT_VALUE_EQ(long double, LongDouble);
    CASE_PORT_VALUE_EQ(QTime, Time);
    CASE_PORT_VALUE_EQ(QDate, Date);
    CASE_PORT_VALUE_EQ(QDateTime, DateTime);
    default:
        ThrowException("Cannot compare Port values of this type").value(nameOf(type));
    }
    return false;
}

bool le(base::PortType type, const void *x, const void *y) {
    return lt(type, x,y) || eq(type, x,y);
}

bool ge(base::PortType type, const void *x, const void *y) {
    return gt(type, x,y) || eq(type, x,y);
}

} // namespace
