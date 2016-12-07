#include "exception.h"
#include "port_buffer.h"

namespace base {

PortBuffer *PortBuffer::_portBuffer = 0;

PortBuffer& portBuffer(QObject *parent) {
    if (!PortBuffer::_portBuffer) {
        PortBuffer::_portBuffer = new PortBuffer(parent);
    }
    return *PortBuffer::_portBuffer;
}

PortBuffer::PortBuffer(QObject *parent)
    : QObject(parent)
{
    Q_ASSERT(parent);
}

#define DELETE_PORT_BUFFER(X) \
for (auto p : _##X) delete p; \
for (auto p : _##X##Vector) delete p

PortBuffer::~PortBuffer() {
    DELETE_PORT_BUFFER(Bool);
    DELETE_PORT_BUFFER(Char);
    DELETE_PORT_BUFFER(Int);
    DELETE_PORT_BUFFER(LongInt);
    DELETE_PORT_BUFFER(LongLongInt);
    DELETE_PORT_BUFFER(Float);
    DELETE_PORT_BUFFER(Double);
    DELETE_PORT_BUFFER(LongDouble);
    DELETE_PORT_BUFFER(String);
    DELETE_PORT_BUFFER(Date);
    DELETE_PORT_BUFFER(Time);
    DELETE_PORT_BUFFER(DateTime);
    PortBuffer::_portBuffer = 0;
}

#define CASE_CREATE_PORT_BUFFER(X,Y) \
    case Y: \
        _##Y << new X; \
        p = _##Y.last(); break; \
    case Y##Vector: \
        _##Y##Vector << new QVector<X>; \
        p = _##Y##Vector.last(); break

void* PortBuffer::createBuffer(PortType type) {
    void *p(0);
    switch (type) {
        CASE_CREATE_PORT_BUFFER(bool, Bool);
        CASE_CREATE_PORT_BUFFER(char, Char);
        CASE_CREATE_PORT_BUFFER(int, Int);
        CASE_CREATE_PORT_BUFFER(long int, LongInt);
        CASE_CREATE_PORT_BUFFER(long long int, LongLongInt);
        CASE_CREATE_PORT_BUFFER(float, Float);
        CASE_CREATE_PORT_BUFFER(double, Double);
        CASE_CREATE_PORT_BUFFER(long double, LongDouble);
        CASE_CREATE_PORT_BUFFER(QString, String);
        CASE_CREATE_PORT_BUFFER(QDate, Date);
        CASE_CREATE_PORT_BUFFER(QTime, Time);
        CASE_CREATE_PORT_BUFFER(QDateTime, DateTime);
    case Null:
        ThrowException("Cannot create port buffer of 'Null' type");
    }
    Q_ASSERT(p);
    return p;
}

}

