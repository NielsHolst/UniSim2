#ifndef PORT_TYPE_H
#define PORT_TYPE_H
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QTime>
#include <QVector>

namespace boxes {

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

//bool isVector(Type x) { return int(x)%2==0; }

template <class T> PortType typeOf() { return Null; }

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

QString nameOf(PortType type);

}

#endif
