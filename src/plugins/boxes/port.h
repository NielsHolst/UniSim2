#ifndef PORT_H
#define PORT_H
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include "assign.h"
#include "convert.h"
#include "exception.h"
#include "port_transform.h"
#include "port_type.h"



namespace boxes {

class Box;

class Port : public QObject {
public:
    enum Access{Read, ReadWrite};
private:
    void *_valuePtr;
    PortType _valueType;
    PortTransform _transform;
    QStringList _importPortPaths;
    QVector<Port *> _importPorts;
    Access _access;
    bool _reset, _initialize;

public:
    // Configure
    Port(QString name="noname", QObject *parent=0);
    template <class T> Port& data(T *valuePtr);
    template <class T> Port& equals(T value);
    Port& equals(const char *value) { return equals(QString(value)); }
    Port& import(QString pathToPort);
    Port& transform(PortTransform t);
    Port& access(Access a);
    Port& zeroAtReset();
    Port& zeroAtInitialize();
    Port& noReset();
    Port& noInitialize();

    // Change
    void resolveImports();
    void reset();
    void initialize();
    void copyFromImport();

    // Access
    template <class T> T value() const;
    template <class T> const T* valuePtr() const;

    // Attributes
    bool hasImport() const;
    PortTransform transform() const;
    Access access() const;
};

template <class T> Port& Port::data(T *valuePtr) {
    _valuePtr = valuePtr;
    _valueType = typeOf<T>();
    return *this;
}

template <class T> Port& Port::equals(T value)
{
    if (_valuePtr == 0)
        throw Exception("Cannot set port value by 'equals' because port 'data' has not been set", "", this);
    _importPortPaths.clear();
    assign(_valueType, _valuePtr, typeOf<T>(), &value, _transform);
    return *this;
}

template <class T> T Port::value() const
{
    T value;
    assign(typeOf<T>(), &value, _valueType, _valuePtr);
    return value;
}

template <class T> const T* Port::valuePtr() const
{
    if (_valueType != typeOf<T>()) {
        QString msg("Port of type %1 cannot be converted to type %2");
        throw Exception(msg.arg(nameOf(_valueType)).arg(nameOf(typeOf<T>())), "", this);
    }
    return reinterpret_cast<const T*>(_valuePtr);
}

}
#endif
