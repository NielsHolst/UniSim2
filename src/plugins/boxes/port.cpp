#include "box.h"
#include "general.h"
#include "initialize.h"
#include "path.h"
#include "port.h"

namespace boxes {

Port::Port(QString name, QObject *parent)
    : QObject(parent), _valuePtr(0), _valueType(Null), _transform(Identity),
      _access(Read), _reset(false), _initialize(false)
{
    Class(Port);
    setObjectName(name);
    Box *boxParent = dynamic_cast<Box*>(parent);
    if (boxParent)
        boxParent->addPort(this);
}

Port& Port::import(QString pathToPort) {
    _importPortPaths << pathToPort;
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

void Port::resolveImports() {
    // Assume only one import (preliminary)
    if (hasImport()) {
        QString p = _importPortPaths.at(0);
        Path path(p, parent());
        Port *port = path.resolveOne<Port>(this);
        _importPorts << port;
    }
}

void Port::reset() {
    if (!hasImport() && _reset)
        boxes::initialize(_valueType, _valuePtr);
}

void Port::initialize() {
    if (!hasImport() && _initialize)
        boxes::initialize(_valueType, _valuePtr);
}


void Port::copyFromImport() {
    // Assume only one import (preliminary)
    if (!hasImport())
        return;
    if (_valuePtr == 0)
        throw Exception("Cannot copy from import because 'data' has not been set in destination port", "", this);
    Port *port =  _importPorts.at(0);
    if (port->_valuePtr == 0)
        throw Exception("Cannot copy from import because 'data' has not been set in source port", "", this);
    assign(_valueType, _valuePtr, port->_valueType, port->_valuePtr, _transform);
}

bool Port::hasImport() const {
    return !_importPortPaths.isEmpty();
}

PortTransform Port::transform() const {
    return _transform;
}

Port::Access Port::access() const {
    return _access;
}

}
