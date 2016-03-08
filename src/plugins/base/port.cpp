#include "box.h"
#include "general.h"
#include "path.h"
#include "port.h"
#include "vectorize.h"

namespace base {

unsigned Port::_trackFlags = Reset | Update;

Port::Port(QString name, QObject *parent, bool orphan)
    : QObject(parent), _valuePtr(0), _valueType(Null), _importPath(""),
      _accessFlags(Read), _reset(false), _track(this)
{
    Class(Port);
    setObjectName(name);
    Box *boxParent = dynamic_cast<Box*>(parent);
    if (boxParent) {
        if (orphan)
            boxParent->addOrphanPort(this);
        else
            boxParent->addPort(this);
    }
    _attributes["format"] = QString();
    _attributes["page"] = QString();
    _attributes["plot"] = QString();
    _attributes["label"] = QString();
    _attributes["transform"] = QString();
}

// Configure

Port& Port::imports(QString pathToPort) {
    if (!(_accessFlags & Write))
        throw Exception("Cannot import port value because port is not for input", "", this);
    _importPath = pathToPort;
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


QStringList Port::attributes() {
    return QStringList( _attributes.keys() );
}


// Set attributes

Port& Port::attribute(QString name, QString value) {
    if (!_attributes.contains(name))
        throw Exception("Unknown attribute", name, this);
    _attributes[name] = value;
    return *this;
}

#define SET_ATTRIBUTE(X) \
Port& Port::X(QString fo) { \
    _attributes[#X] = fo; \
    return *this; \
}

SET_ATTRIBUTE(format)
SET_ATTRIBUTE(page)
SET_ATTRIBUTE(plot)
SET_ATTRIBUTE(label)
SET_ATTRIBUTE(transform)

Port& Port::transform(PortTransform tr) {
    return transform(convert<QString>(tr));
}

// Get attributes

QString Port::attribute(QString name) const {
    if (!_attributes.contains(name))
        throw Exception("Unknown attribute", name, this);
    return _attributes.value(name);
}

#define GET_ATTRIBUTE(X) \
QString Port::X() const { \
    return _attributes.value(#X); \
}

GET_ATTRIBUTE(format)
GET_ATTRIBUTE(page)
GET_ATTRIBUTE(plot)
GET_ATTRIBUTE(label)
GET_ATTRIBUTE(transform)

// Change

void Port::resolveImports() {
    if (_importPath.isEmpty())
        return;
    Box *context = boxParent();
    _importPorts = Path(_importPath, context).resolve<Port>(-1,this);
    if (_importPorts.isEmpty())
        throw Exception("No matching import ports found", _importPath, this);
    _importType = commonType(_importPorts);
    if (_importType == Null)
        _importType = _valueType;
}

void Port::allocatePortBuffer() {
    if (doTrack()) {
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
    PortTransform pt = convert<PortTransform>(transform());
    try {
        if (sources.size() == 1) {
            const Port *source = sources.at(0);
            base::assign(_valueType, _valuePtr, source->_valueType, source->_valuePtr, pt, this);
        }
        else {
            const void *sourceVector = vectorize(_importType, sources);
            base::assign(_valueType, _valuePtr, asVector(_importType), sourceVector, pt, this);
        }
    }
    catch (Exception &ex) {
        throw Exception(ex.message(), ex.value(), this);
    }
}

void Port::track(Step step) {
    if (doTrack() && (step & _trackFlags))
        _track.append(_valuePtr);
}

void Port::format(PortType type) {
    switch (type) {
        case Date:
        case DateVector:
            format("ymd");
            break;
        case Time:
        case TimeVector:
            format("hms");
            break;
        case DateTime:
        case DateTimeVector:
            format("ymdhms");
            break;
        default:
            format("NA");
    }
}

// Access

Box *Port::boxParent() {
    Box *par = dynamic_cast<Box*>(parent());
    if (!par)
        throw Exception("Application error: Port has no Box parent", "", this);
    return par;
}

bool Port::hasValue() const {
    return _valuePtr != 0;
}

template <> const void* Port::valuePtr() const {
    return _valuePtr;
}

const Vector* Port::trackPtr() const {
    return doTrack() ? &_track : 0;
}

PortType Port::type() const {
    return _valueType;
}

unsigned Port::accessFlags() const {
    return _accessFlags;
}

bool Port::doTrack() const {
    return !(attribute("plot").isNull() && attribute("page").isNull());
}

bool Port::hasImport() const {
    return !_importPath.isEmpty();
}

QString Port::importPath() const {
    return _importPath;
}

QVector<Port*> Port::importPorts() const {
    return _importPorts;
}

QVector<Port*> Port::trackedPorts() {
    QVector<base::Port*> result, all = Path("*{Port}").resolve<Port>();
    for (Port *port : all) {
        if (port->trackPtr())
            result << port;
    }
    return result;
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
