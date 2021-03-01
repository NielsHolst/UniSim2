/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <limits>
#include <stdlib.h>
#include <string>
#include <QDate>
#include <QDateTime>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QVector>
#include "assign.h"
#include "convert.h"
#include "exception.h"

using std::max;
using std::min;
using std::numeric_limits;

namespace {
    const QObject *_context;
}

namespace base {

#define DEST_PTR(destT) \
    reinterpret_cast<destT*>(destPtr)

#define DEST(destT) \
    (*(reinterpret_cast<destT*>(destPtr)))

#define SOURCE_PTR(sourceT) \
    reinterpret_cast<const sourceT*>(sourcePtr)

#define SOURCE(sourceT) \
    (*(reinterpret_cast<const sourceT*>(sourcePtr)))

template <class destT, class sourceT>
void assignToScalarFromScalar(void *destPtr, const void *sourcePtr, PortTransform) {
    DEST(destT) = convert<destT>(SOURCE(sourceT));
}

template <class destT, class sourceT>
void assignToScalarFromVector(void *destPtr, const void *sourcePtr, PortTransform transform) {
    const QVector<sourceT> *sourceVector = SOURCE_PTR(QVector<sourceT>);
    const sourceT *sourceData = sourceVector->data();
    int i = 0, n = sourceVector->size();
    sourceT value = sourceT();
    switch (transform) {
    case Identity:
        if (n != 1)
            ThrowException("Cannot assign to scalar from vector. Expected vector size = 1").value(n).context(_context);
        value = *sourceData;
        break;
    case Sum:
        value = 0;
        while (i++<n)
            value += (*sourceData++);
        break;
    case Prod:
        value = 1;
        while (i++<n)
            value *= (*sourceData++);
        break;
    case Mean:
        value = 0;
        while (i++<n)
            value += (*sourceData++);
        value /= n;
        break;
    case Min:
        value = numeric_limits<sourceT>::max();
        while (i++<n)
            value = min(value, *sourceData++);
        break;
    case Max:
        value = numeric_limits<sourceT>::min();
        while (i++<n)
            value = max(value, *sourceData++);
        break;
    case All:
        value = true;
        while (value && i++<n)
            value = value && (*sourceData++);
        break;
    case Any:
        value = false;
        while (!value && i++<n)
            value = value || (*sourceData++);
        break;
    case Copy:
    case Split:
        ThrowException("Transform cannot be applied when assigning a vector to a scalar").value(convert<QString>(transform)).context(_context);
    }
    DEST(destT) = convert<destT>(value);
}

#define ASSIGN_SCALAR_TO_VECTOR_SIZE_1(destT, sourceT) \
template <> void assignToScalarFromVector<destT, sourceT>(void *destPtr, const void *sourcePtr, PortTransform transform) { \
    const QVector<sourceT> *sourceVector = SOURCE_PTR(QVector<sourceT>); \
    int n = sourceVector->size(); \
    switch (transform) { \
    case Identity: \
        if (n != 1) \
            ThrowException("Cannot assign to scalar from vector. Expected vector size = 1").value(convert<QString>(*sourceVector)).context(_context); \
        DEST(destT) = convert<destT>(sourceVector->at(0)); \
        break; \
    default: \
        ThrowException("Transform cannot be applied when assigning a vector to a scalar").value(convert<QString>(transform)).context(_context); \
    } \
}

#define ASSIGN_ANY_SCALAR_TO_VECTOR_SIZE_1(sourceT) \
ASSIGN_SCALAR_TO_VECTOR_SIZE_1(bool, sourceT) \
ASSIGN_SCALAR_TO_VECTOR_SIZE_1(char, sourceT) \
ASSIGN_SCALAR_TO_VECTOR_SIZE_1(int, sourceT) \
ASSIGN_SCALAR_TO_VECTOR_SIZE_1(long int, sourceT) \
ASSIGN_SCALAR_TO_VECTOR_SIZE_1(long long int, sourceT) \
ASSIGN_SCALAR_TO_VECTOR_SIZE_1(float, sourceT) \
ASSIGN_SCALAR_TO_VECTOR_SIZE_1(double, sourceT) \
ASSIGN_SCALAR_TO_VECTOR_SIZE_1(long double, sourceT) \
ASSIGN_SCALAR_TO_VECTOR_SIZE_1(QString, sourceT) \
ASSIGN_SCALAR_TO_VECTOR_SIZE_1(QDate, sourceT) \
ASSIGN_SCALAR_TO_VECTOR_SIZE_1(QTime, sourceT) \
ASSIGN_SCALAR_TO_VECTOR_SIZE_1(QDateTime, sourceT)
//
ASSIGN_ANY_SCALAR_TO_VECTOR_SIZE_1(QString)
ASSIGN_ANY_SCALAR_TO_VECTOR_SIZE_1(QDate)
ASSIGN_ANY_SCALAR_TO_VECTOR_SIZE_1(QTime)
ASSIGN_ANY_SCALAR_TO_VECTOR_SIZE_1(QDateTime)

// Bypass division for Split transform below
QString operator/(QString x, int) { return x; }
QDate operator/(QDate x, int) { return x; }
QTime operator/(QTime x, int) { return x; }
QDateTime operator/(QDateTime x, int) { return x; }

template <class sourceT>
QStringList asStringList(const void *) {
    return QStringList();
}

template <>
QStringList asStringList<QString>(const void *sourcePtr) {
    QString source = SOURCE(QString);
    return isParenthesized(source) ? split(source) : QStringList();
}

template <class destT, class sourceT>
void assignToVectorFromScalar(void *destPtr, const void *sourcePtr, PortTransform transform) {
    if (transform != Identity)
        ThrowException("Expected 'Identity' transform when assigning to vector").value(convert<QString>(transform)).context(_context);
    QVector<destT> *destVector = DEST_PTR(QVector<destT>);
    sourceT source = SOURCE(sourceT);
    destT destValue = convert<destT>(source);

    int n = destVector->size();
    if (n <= 1)
        destVector->fill(destValue, 1);
    else if (transform == Copy)
        destVector->fill(destValue);
    else if (transform == Split) {
        if (n>0) destVector->fill(convert<destT>(source/n));
    }
    else {
        QString msg("When assigning a scalar to a vector, vector size must be 0 or 1,"
                    "\nor scalar must be transformed by Copy or Split"),
                value("Vector size = %1, Transform = %2");
        ThrowException(msg).value(value.arg(QString::number(n)).arg(convert<QString>(transform))).context(_context);
    }
}

#define ASSIGN_TO_VECTOR_FROM_VECTOR_INTRO(destT, sourceT) \
    if (transform != Identity) \
        ThrowException("Expected 'Identity' transform when assigning Vector to Vector").value(convert<QString>(transform)).context(_context); \
    QVector<destT> *destVector = DEST_PTR(QVector<destT>); \
    const QVector<sourceT> *sourceVector = SOURCE_PTR(QVector<sourceT>);

template <class destT, class sourceT>
void assignToVectorFromVector(void *destPtr, const void *sourcePtr, PortTransform transform) {
    ASSIGN_TO_VECTOR_FROM_VECTOR_INTRO(destT, sourceT);
    const sourceT *sourceData = sourceVector->data();
    int i = 0,
        n = sourceVector->size();
    destVector->resize(n);
    destT *destData = destVector->data();
    while (i++<n)
        *destData++ = convert<destT>(*sourceData++);
}

#define ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(T) \
template <> \
void assignToVectorFromVector<T,T>(void *destPtr, const void *sourcePtr, PortTransform transform) { \
    ASSIGN_TO_VECTOR_FROM_VECTOR_INTRO(T, T); \
    *destVector = *sourceVector; \
}

ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(bool)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(char)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(int)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(long int)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(long long int)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(float)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(double)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(long double)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(QString)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(QDate)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(QTime)
ASSIGN_TO_VECTOR_FROM_VECTOR_SAME_TYPE(QDateTime)

#define ASSIGN_TO_SCALAR(destT, sourceT, sourcePortT, destPtr, sourcePtr, transform) \
case sourcePortT: \
    assignToScalarFromScalar<destT,sourceT>(destPtr, sourcePtr, transform); \
    break; \
case sourcePortT##Vector: \
    assignToScalarFromVector<destT,sourceT>(destPtr, sourcePtr, transform); \
    break;

#define ASSIGN_TO_VECTOR(destT, sourceT, sourcePortT, destPtr, sourcePtr, transform) \
case sourcePortT: \
    assignToVectorFromScalar<destT,sourceT>(destPtr, sourcePtr, transform); \
    break; \
case sourcePortT##Vector: \
    assignToVectorFromVector<destT,sourceT>(destPtr, sourcePtr, transform); \
    break;

#define CANNOT_ASSIGN_FROM_NULL \
case Null: \
    ThrowException("Cannot assign from this type").value(nameOf(sourceT)).context(_context); \
    break;

#define CASE_ASSIGN_TO_SCALAR(destT) \
switch (sourceT) { \
    ASSIGN_TO_SCALAR(destT, bool, Bool, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, char, Char, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, int, Int, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, long int, LongInt, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, long long int, LongLongInt, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, float, Float, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, double, Double, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, long double, LongDouble, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, QString, String, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, QDate, Date, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, QTime, Time, destPtr, sourcePtr, transform) \
    ASSIGN_TO_SCALAR(destT, QDateTime, DateTime, destPtr, sourcePtr, transform) \
    CANNOT_ASSIGN_FROM_NULL \
}

#define CASE_ASSIGN_TO_VECTOR(destT) \
switch (sourceT) { \
    ASSIGN_TO_VECTOR(destT, bool, Bool, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, char, Char, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, int, Int, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, long int, LongInt, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, long long int, LongLongInt, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, float, Float, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, double, Double, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, long double, LongDouble, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, QString, String, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, QDate, Date, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, QTime, Time, destPtr, sourcePtr, transform) \
    ASSIGN_TO_VECTOR(destT, QDateTime, DateTime, destPtr, sourcePtr, transform) \
    CANNOT_ASSIGN_FROM_NULL \
}

#define CASE_ASSIGN(destPortType, destCppType) \
case destPortType: \
    CASE_ASSIGN_TO_SCALAR(destCppType) \
    break; \
case destPortType##Vector: \
    CASE_ASSIGN_TO_VECTOR(destCppType) \
    break

void assign(PortType destT, void *destPtr, PortType sourceT, const void *sourcePtr, PortTransform transform, const QObject *context) {
    _context = context;
    if (destPtr==nullptr)
        ThrowException("Port data has not been set").context(_context);
    if (sourcePtr==nullptr)
        ThrowException("Port source of import has not been set").context(_context);

    // Split
    if (sourceT == String && isParenthesized(SOURCE(QString))) {
        QVector<QString> vector = split(SOURCE(QString)).toVector();
        if (!vector.isEmpty())
            assign(destT, destPtr, StringVector, &vector, transform, context);
    }
    else {
        switch (destT) {
            CASE_ASSIGN(Bool, bool);
            CASE_ASSIGN(Char, char);
            CASE_ASSIGN(Int, int);
            CASE_ASSIGN(LongInt, long int);
            CASE_ASSIGN(LongLongInt, long long int);
            CASE_ASSIGN(Float, float);
            CASE_ASSIGN(Double, double);
            CASE_ASSIGN(LongDouble, long double);
            CASE_ASSIGN(String, QString);
            CASE_ASSIGN(Date, QDate);
            CASE_ASSIGN(Time, QTime);
            CASE_ASSIGN(DateTime, QDateTime);
        case Null:
            ThrowException("Cannot assign to Null");
        }
    }
}

} //namespace
