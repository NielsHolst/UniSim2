#ifndef VECTOR_H
#define VECTOR_H
#include "port_type.h"

namespace base {

class Port;

class Vector {
public:
    Vector(Port *parent);
    ~Vector();
    void reserve(int size);
    void erase();
    void append(const void *valuePtr);
    QString toString(int row, QString separator = "\t") const;
    int size() const;
    PortType type() const;
    const void *ptr() const;
private:
    PortType _vectorType;
    void *_vectorPtr;
    Port *_parent;
};

}
#endif
