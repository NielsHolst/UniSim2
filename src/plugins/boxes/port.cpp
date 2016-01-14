#include "box.h"
#include "general.h"
#include "initialize.h"
#include "path.h"
#include "port.h"
#include "vectorize.h"

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
    if (_importPortPaths.isEmpty())
        return;
    _importPorts = Path(_importPortPaths).resolve<Port>(-1,this);
    if (_importPorts.isEmpty())
        throw Exception("No matching import ports found", _importPortPaths.join(" "), this);
    _importType = commonType(_importPorts);
    if (_importType == Null)
        _importType = _valueType;
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
    if (!hasImport())
        return;
    assign(_importPorts);
}

void Port::assign(const QVector<Port*> &sources) {
    if (sources.size() == 1) {
        const Port *source = sources.at(0);
        boxes::assign(_valueType, _valuePtr, source->_valueType, source->_valuePtr, _transform, this);
    }
    else {
        const void *sourceVector = vectorize(_importType, sources);
        boxes::assign(_valueType, _valuePtr, asVector(_importType), sourceVector, _transform, this);
    }
}

PortType Port::type() const {
    return _valueType;
}

PortTransform Port::transform() const {
    return _transform;
}

Port::Access Port::access() const {
    return _access;
}

bool Port::hasImport() const {
    return !_importPortPaths.isEmpty();
}

template <> const void* Port::valuePtr() const {
    return _valuePtr;
}

PortType Port::commonType(const QVector<Port*> &ports) {
    PortType commonType = ports.at(0)->_valueType;
    for (Port *port : ports) {
        if (port->_valueType != commonType) {
            commonType = Null;
            break;
        }
    }
    return commonType;
}

}
