#include "port_type.h"

namespace boxes {

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

}