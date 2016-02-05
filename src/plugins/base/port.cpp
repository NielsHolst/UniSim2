#include "box.h"
#include "general.h"
#include "path.h"
#include "port.h"
#include "vectorize.h"

namespace base {

unsigned Port::_trackFlags = Reset | Update;

Port::Port(QString name, QObject *parent)
    : QObject(parent), _valuePtr(0), _valueType(Null), _transform(Identity),
      _accessFlags(Read), _reset(false),
      _track(this), _trackOn(true)
{
    Class(Port);
    setObjectName(name);
    Box *boxParent = dynamic_cast<Box*>(parent);
    if (boxParent)
        boxParent->addPort(this);
}

Port& Port::import(QString pathToPort) {
    if (!(_accessFlags & Write))
        throw Exception("Cannot import port value because port is not for input", "", this);
    _importPortPaths << pathToPort;
    return *this;
}

Port& Port::transform(PortTransform t) {
    _transform = t;
    return *this;
}

Port& Port::access(unsigned accessFlags) {
    _accessFlags = accessFlags;
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

Port& Port::trackOn() {
    _trackOn = true;
    return *this;
}

Port& Port::trackOff() {
    _trackOn = false;
    return *this;
}

void Port::resolveImports() {
    if (_importPortPaths.isEmpty())
        return;
    Box *context = boxParent();
    _importPorts = Path(_importPortPaths, context).resolve<Port>(-1,this);
    if (_importPorts.isEmpty())
        throw Exception("No matching import ports found", _importPortPaths.join(" "), this);
    _importType = commonType(_importPorts);
    if (_importType == Null)
        _importType = _valueType;
}

void Port::allocatePortBuffer() {
    if (_trackOn) {
         Box *root = boxParent()->currentRoot();
        Port *iterations = root->peakPort("iterations"),
             *steps = root->peakPort("steps");
        int ite = iterations ? iterations->value<int>() : 1,
            ste = steps ? steps->value<int>() : 1;
        _track.reserve(ite*ste);
    }
}

void Port::reset() {
    if (!hasImport() && _reset)
        base::initialize(_valueType, _valuePtr);
}

void Port::copyFromImport() {
    if (!hasImport())
        return;
    assign(_importPorts);
}


void Port::assign(const QVector<Port*> &sources) {
    try {
        if (sources.size() == 1) {
            const Port *source = sources.at(0);
            base::assign(_valueType, _valuePtr, source->_valueType, source->_valuePtr, _transform, this);
        }
        else {
            const void *sourceVector = vectorize(_importType, sources);
            base::assign(_valueType, _valuePtr, asVector(_importType), sourceVector, _transform, this);
        }
    }
    catch (Exception &ex) {
        throw Exception(ex.message(), ex.value(), this);
    }
}

void Port::track(Step step) {
    if (_trackOn && (step & _trackFlags))
        _track.append(_valuePtr);
}

Box *Port::boxParent() {
    Box *par = dynamic_cast<Box*>(parent());
    if (!par)
        throw Exception("Application error: Port has no Box parent", "", this);
    return par;
}

bool Port::hasValue() const {
    return _valuePtr != 0;
}

const Vector* Port::trackPtr() const {
    return _trackOn ? & _track : 0;
}

PortType Port::type() const {
    return _valueType;
}

PortTransform Port::transform() const {
    return _transform;
}

unsigned Port::accessFlags() const {
    return _accessFlags;
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
