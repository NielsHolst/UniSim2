#include "box.h"
#include "general.h"
#include "initialize.h"
#include "port.h"

namespace boxes {

Port::Port(QString name, QObject *parent)
    : QObject(parent), _valuePtr(0), _valueType(Null), _transform(Identity), _importPort(0),
      _access(Read), _reset(false), _initialize(false)
{
    Class(Port);
    setObjectName(name);
    Box *boxParent = dynamic_cast<Box*>(parent);
    if (boxParent)
        boxParent->addPort(this);
}

Port& Port::import(Port *port) {
    _importPort = port;
    return *this;
}

Port& Port::transform(PortTransform t) {
    _transform = t;
    return *this;
}

Port& Port::access(Access a) {
    _access = a;
    return *this;
}

Port& Port::zeroAtInitialize() {
    _initialize = true;
    return *this;
}

Port& Port::noInitialize() {
    _initialize = false;
    return *this;
}

Port& Port::zeroAtReset() {
    _reset = true;
    return *this;
}

Port& Port::noReset() {
    _reset = false;
    return *this;
}

void Port::reset() {
    if (!_importPort && _reset)
        boxes::initialize(_valueType, _valuePtr);
}

void Port::initialize() {
    if (!_importPort && _initialize)
        boxes::initialize(_valueType, _valuePtr);
}


void Port::copyFromImport() {
    if (_importPort == 0)
        return;
    if (_valuePtr == 0)
        throw Exception("Cannot copy from import because 'data' has not been set in destination port", "", this);
    if (_importPort->_valuePtr == 0)
        throw Exception("Cannot copy from import because 'data' has not been set in source port", "", this);
    assign(_valueType, _valuePtr, _importPort->_valueType, _importPort->_valuePtr, _transform);
}

PortTransform Port::transform() const {
    return _transform;
}

Port::Access Port::access() const {
    return _access;
}

}
