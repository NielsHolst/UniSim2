/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PORT_TYPE_H
#define BASE_PORT_TYPE_H
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QVector>
#include "exception.h"

namespace base {

#define TYPEDEF(X,Y) typedef QVector<X> Y;
TYPEDEF(bool, VectorBool)
TYPEDEF(char, VectorChar)
TYPEDEF(int, VectorInt)
TYPEDEF(long int, VectorLongInt)
TYPEDEF(long long int, VectorLongLongInt)
TYPEDEF(float, VectorFloat)
TYPEDEF(double, VectorDouble)
TYPEDEF(long double, VectorLongDouble)
TYPEDEF(QString, VectorString)
TYPEDEF(QDate, VectorDate)
TYPEDEF(QTime, VectorTime)
TYPEDEF(QDateTime, VectorDateTime)

#define TYPE(X) X, X##Vector,
enum PortType {
    TYPE(Bool)
    TYPE(Char)
    TYPE(Int)
    TYPE(LongInt)
    TYPE(LongLongInt)
    TYPE(Float)
    TYPE(Double)
    TYPE(LongDouble)
    TYPE(String)
    TYPE(Date)
    TYPE(Time)
    TYPE(DateTime)
    Null
};

template <class T> PortType typeOf() { return Null; }
template <> inline PortType typeOf<QStringList>() { ThrowException("Use QVector<QString> instead of QStringList for ports"); }
#define TYPEOF(X,Y) \
    template <> inline PortType typeOf<X>() { return Y; } \
    template <> inline PortType typeOf<Vector##Y>() { return Y##Vector; }

TYPEOF(bool, Bool)
TYPEOF(char, Char)
TYPEOF(int, Int)
TYPEOF(long int, LongInt)
TYPEOF(long long int, LongLongInt)
TYPEOF(float, Float)
TYPEOF(double, Double)
TYPEOF(long double, LongDouble)
TYPEOF(QString, String)
TYPEOF(QDate, Date)
TYPEOF(QTime, Time)
TYPEOF(QDateTime, DateTime)

template<class T> T convert(PortType )  { ThrowException("Can only convert port type  to QString"); }
template<> QString convert(PortType type);

QString nameOf(PortType type);
bool isScalar(PortType type);
bool isVector(PortType type);
PortType asVector(PortType type);
PortType asScalar(PortType type);
PortType deducePortType(QString s);

}

#endif
