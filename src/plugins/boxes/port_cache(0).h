#ifndef PORT_CACHE_H
#define PORT_CACHE_H
#include <memory>
#include <QMap>
#include <QObject>
#include <QVector>
#include "port_type.h"

#define CACHE_VAR(X, Y) \
    X v##Y; \
    QVector<X> vv##Y;

namespace boxes {

class Port;

class PortCache : public QObject
{
public:
    PortCache();
    Port* port(PortType type);
private:
    // Singleton object
    static std::unique_ptr<PortCache> _portCache;
    friend PortCache* portCache();

    // Data
    QMap<PortType, Port*> cache;
    CACHE_VAR(bool, Bool)
    CACHE_VAR(char, Char)
    CACHE_VAR(int, Int)
    CACHE_VAR(long int, LongInt)
    CACHE_VAR(long long int, LongLongInt)
    CACHE_VAR(float, Float)
    CACHE_VAR(double, Double)
    CACHE_VAR(long double, LongDouble)
    CACHE_VAR(QString, String)
    CACHE_VAR(QDate, Date)
    CACHE_VAR(QTime, Time)
    CACHE_VAR(QDateTime, DateTime)
};

PortCache* portCache();

}
#endif
