#include <QList>
#include <QVector>
#include "convert.h"
#include "exception.h"
#include "port_type.h"

namespace base {

#define NAMEOF_PORTTYPE(Y,Z) \
    case Z: \
        s = #Y; \
        break; \
    case Z##Vector: \
        s = "QVector<" #Y ">"; \
        break;

QString nameOf(PortType type) {
    QString s;
    switch (type) {
        NAMEOF_PORTTYPE(bool, Bool)
        NAMEOF_PORTTYPE(char, Char)
        NAMEOF_PORTTYPE(int, Int)
        NAMEOF_PORTTYPE(long int, LongInt)
        NAMEOF_PORTTYPE(long long int, LongLongInt)
        NAMEOF_PORTTYPE(float, Float)
        NAMEOF_PORTTYPE(double, Double)
        NAMEOF_PORTTYPE(long double, LongDouble)
        NAMEOF_PORTTYPE(QString, String)
        NAMEOF_PORTTYPE(QDate, Date)
        NAMEOF_PORTTYPE(QTime, Time)
        NAMEOF_PORTTYPE(QDateTime, DateTime)
        case Null:
            s = "Null";
    }
    return s;
}

bool isVector(PortType type) {
    return int(type)%2 == 1;
}

PortType asVector(PortType type) {
    if (type==Null)
        ThrowException("Cannot convert 'Null' to vector type");
    int i(type);
    return (i%2 == 0) ? PortType(i+1) : type;
}

template<class T> inline bool isType(QString s) {
    try {
        convert<T>(s);
        return true;
    }
    catch(Exception &) {
    }
    return false;
}

PortType deducePortType(QString s) {
    if (isType<bool>(s)) return Bool;
    if (isType<char>(s)) return Char;
    if (isType<int>(s)) return Int;
    if (isType<long int>(s)) return LongInt;
    if (isType<long long int>(s)) return LongLongInt;
    if (isType<double>(s)) return Double;
    if (isType<long double>(s)) return LongDouble;
    if (isType<QDate>(s)) return Date;
    if (isType<QDateTime>(s)) return DateTime;
    if (isType<QTime>(s)) return Time;
    return String;
}

}
