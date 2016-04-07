#ifndef PORT_BUFFER_H
#define PORT_BUFFER_H
#include <QList>
#include <QObject>
#include <QVector>
#include "port_type.h"

#define DECLARE_PORT_BUFFER(X,Y) \
    QList<X*> _##Y; \
    QList<QVector<X>*> _##Y##Vector

namespace base {

class Box;
class Command;

class PortBuffer : public QObject {
public:
    PortBuffer();
    ~PortBuffer();
    void* createBuffer(PortType type);
private:
    // Data
    DECLARE_PORT_BUFFER(bool, Bool);
    DECLARE_PORT_BUFFER(char, Char);
    DECLARE_PORT_BUFFER(int, Int);
    DECLARE_PORT_BUFFER(long int, LongInt);
    DECLARE_PORT_BUFFER(long long int, LongLongInt);
    DECLARE_PORT_BUFFER(float, Float);
    DECLARE_PORT_BUFFER(double, Double);
    DECLARE_PORT_BUFFER(long double, LongDouble);
    DECLARE_PORT_BUFFER(QString, String);
    DECLARE_PORT_BUFFER(QDate, Date);
    DECLARE_PORT_BUFFER(QTime, Time);
    DECLARE_PORT_BUFFER(QDateTime, DateTime);
    // Singleton
    static PortBuffer *_portBuffer;
    friend PortBuffer& portBuffer();
    // Methods
};

PortBuffer& portBuffer();

}

#endif
