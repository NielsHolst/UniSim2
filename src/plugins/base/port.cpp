#include "box.h"
#include "general.h"
#include "path.h"
#include "port.h"
#include "vectorize.h"

namespace base {

unsigned Port::_trackFlags = Reset | Update;

Port::Port(QString name, QObject *parent, bool orphan)
    : QObject(parent), _valuePtr(0), _valueType(Null),
      _importPath(""), _importPortMustExist(true),
      _access(Input), _reset(false), _doTrack(false), _track(this)
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
}

// Configure

Port& Port::equals(const char *value) {
    return equals(QString(value));
}

Port& Port::equals(QStringList value) {
    return equals(value.toVector());
}

Port& Port::imports(QString pathToPort) {
    _importPath = pathToPort;
    _importPortMustExist = true;
    return help("Defaults to " + pathToPort);
}

Port& Port::importsMaybe(QString pathToPort) {
    _importPath = pathToPort;
    _importPortMustExist = false;
    return help("Defaults to " + pathToPort + " (if it exists)");
}

Port& Port::access(Access acc) {
    _access = acc;
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
    static QStringList attr;
    if (attr.isEmpty()) {
        attr << "format" << "page" << "plot" << "label" << "transform";
    }
    return attr;
}


// Set attributes

#define CASE_SET_ATTRIBUTE(X,Y) \
if (name == #X) { \
    _attributes.X = Y; \
    return *this; \
}

Port& Port::attribute(QString name, QString value) {
    CASE_SET_ATTRIBUTE(format, value)
    CASE_SET_ATTRIBUTE(page, value)
    CASE_SET_ATTRIBUTE(plot, value)
    CASE_SET_ATTRIBUTE(label, value)
    CASE_SET_ATTRIBUTE(help, value)
    if (name == "transform") {
        _attributes.transform = value;
        _attributes.portTransform = convert<PortTransform>(value);
        return *this;
    }
    ThrowException("Unknown attribute").value(name).context(this);
}

#define SET_ATTRIBUTE(X) \
Port& Port::X(QString value) { \
    return attribute(#X, value); \
}

SET_ATTRIBUTE(format)
SET_ATTRIBUTE(page)
SET_ATTRIBUTE(plot)
SET_ATTRIBUTE(label)
SET_ATTRIBUTE(transform)
SET_ATTRIBUTE(help)

Port& Port::transform(PortTransform tr) {
    return attribute("transform", convert<QString>(tr));
}

// Get attributes

#define CASE_GET_ATTRIBUTE(X) if (name == #X) return _attributes. X

QString Port::attribute(QString name) const {
    CASE_GET_ATTRIBUTE(format);
    CASE_GET_ATTRIBUTE(page);
    CASE_GET_ATTRIBUTE(plot);
    CASE_GET_ATTRIBUTE(label);
    CASE_GET_ATTRIBUTE(transform);
    CASE_GET_ATTRIBUTE(help);
    ThrowException("Unknown attribute").value(name).context(this);
}

#define GET_ATTRIBUTE(X) \
QString Port::X() const { \
    return _attributes. X; \
}

GET_ATTRIBUTE(format)
GET_ATTRIBUTE(page)
GET_ATTRIBUTE(plot)
GET_ATTRIBUTE(label)
GET_ATTRIBUTE(help)

QStringList Port::labelList() const {
    QStringList list;
    int n = valueSize();
    if (n == 1)
        list << _attributes.label;
    else {
        for (int i = 0; i < n; ++i)
            list << (_attributes.label + "_" + QString::number(i));
    }
    return list;
}

PortTransform Port::transform() const {
    return _attributes.portTransform;
}

// Change

namespace {
    PortType deduceTypeFromImportType(PortType importType, PortTransform transform) {
        if (isScalar(importType))
            return importType;

        PortType type{Null};
        switch (transform) {
            case Identity:
                type = importType;
                break;
            case Sum:
            case Average:
            case Min:
            case Max:
            case All:
            case Any:
                type = asScalar(importType);
                break;
            case Copy:
            case Split:
                ThrowException("Transform cannot be applied on a vector").value(transform);
        }
        return type;
    }
}

void Port::resolveImports() {
    // Find import ports defined by the import path
    if (_importPath.isEmpty())
        return;
    Box *context = boxParent();
    Path path = Path(_importPath, context);
    _importPorts = path.resolveMany<Port>();

    // Missing import ports may be an error, otherwise accept and clear the import path
    if (_importPorts.isEmpty()) {
        if (_importPortMustExist)
            ThrowException("No matching import ports found").value(_importPath).context(this);
        else {
            _importPath.clear();
            return;
        }
    }

    // Import port must have a type
    _importType = commonType(_importPorts);
    if (_importType == Null)
        ThrowException("Import port is of unknown type").value(_importPath).context(this);

    // Deduce value type from import type (this happens of port is an extra, added port)
    if (_valueType == Null) {
        _valueType = deduceTypeFromImportType(_importType, transform());
        if (_valueType == Null)
            ThrowException("Unexpected error: Type of imported value is Null");
//        if (_importPorts.size() > 1 && transform() != Identity)
//            _valueType = asVector(_valueType);
    }

    // Check update order relative to import
    /*
    for (Port *port : _importPorts) {
        int myOrder = context->order(),
            importOrder = port->boxParent()->order();
        QString s = "%1(#%2) imports %3(#%4)";
        QString msg = s.arg(fullName(this)).arg(myOrder).arg(fullName(port)).arg(importOrder);
        if (myOrder == importOrder)
            _warnings << "Warning: Port imports sibling port but the update order of siblings is undefined\n" + msg;
        else if (myOrder < importOrder)
            _warnings << "Warning: Port imports a port that will be updated after this port\n" + msg;
    }
    */
}

void Port::allocatePortBuffer() {
    _doTrack = !(attribute("plot").isNull() && attribute("page").isNull());
    if (_doTrack) {
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
    // Create buffer for value if necessary
    if (!_valuePtr) {
        if (_valueType == Null) {
            _valueType = sources.at(0)->type();
            if (sources.size() > 1 && transform() != Identity)
                _valueType = asVector(_valueType);
        }
        _valuePtr = portBuffer().createBuffer(_valueType);
    }
    // Assign as scalar or vector
    if (sources.size() == 1) {
        const Port *source = sources.at(0);
        base::assign(_valueType, _valuePtr, source->_valueType, source->_valuePtr, transform(), this);
    }
    else {
        const void *sourceVector = vectorize(_importType, sources);
        base::assign(_valueType, _valuePtr, asVector(_importType), sourceVector, transform(), this);
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
        ThrowException("Application error: Port has no Box parent").context(this);
    return par;
}

bool Port::hasValue() const {
    return _valuePtr != 0;
}

#define CASE_VALUE_SIZE(X,Y) \
    case Y##Vector: \
        return reinterpret_cast<QVector<X>*>(_valuePtr)->size(); \
        break

int Port::valueSize() const {
    switch (_valueType) {
    CASE_VALUE_SIZE(bool, Bool);
    CASE_VALUE_SIZE(char, Char);
    CASE_VALUE_SIZE(int, Int);
    CASE_VALUE_SIZE(long int, LongInt);
    CASE_VALUE_SIZE(long long int, LongLongInt);
    CASE_VALUE_SIZE(float, Float);
    CASE_VALUE_SIZE(double, Double);
    CASE_VALUE_SIZE(long double, LongDouble);
    CASE_VALUE_SIZE(QString, String);
    CASE_VALUE_SIZE(QDate, Date);
    CASE_VALUE_SIZE(QDateTime, DateTime);
    CASE_VALUE_SIZE(QTime, Time);
    default:
        return 1;
    }
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

Port::Access Port::access() const {
    return _access;
}

bool Port::doTrack() const {
    return _doTrack;
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

QStringList Port::warnings() const {
    return _warnings;
}

QVector<Port*> Port::trackedPorts() {
    QVector<base::Port*> result, all = Path("*{Port}").resolveMany<Port>();
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
