#include <port.h>
#include "assign.h"
#include "box.h"
#include "environment.h"
#include "exception.h"
#include "port_value_op.h"
#include "track.h"
#include "unique_name.h"

namespace base {

//
// Global
//

QSet<Track::Order> Track::_orders;
Track::Tracks Track::_tracks;

bool operator==(const Track::Order &a, const Track::Order &b) {
    return a.port==b.port && a.filter==b.filter;
}

bool operator<(const Track::Order &a, const Track::Order &b) {
    return a.port<b.port || a.filter<b.filter;
}

uint qHash(Track::Order order) {
    return qHash(qMakePair(order.port, static_cast<int>(order.filter)));
}

//
// Track
//

Track::Track(Order order)
    : QObject(order.port),
      _port(order.port), _filter(order.filter),
      _buffer(order.port),
      _filteredValuePtr(0),
      _count(0), _allocated(false)
{
}

void Track::allocateBuffer() {
    if (_allocated) return;
    Box *root = environment().root();
    Port *iterations = root->peakPort("iterations"),
         *steps = root->peakPort("steps");
    int numIterations = iterations ? iterations->value<int>() : 1,
        numSteps = steps ? steps->value<int>() : 1,
        numTotal = (_filter == PortFilter::None) ? numIterations*numSteps : numIterations;
    _buffer.reserve(numTotal);
    _allocated = true;
}

Track::~Track() {
    port_value_op::deallocate(_type, _filteredValuePtr);
}

Track::Order Track::takeOrder(Port *port, PortFilter filter) {
    Order order = Order{port, filter};
    _orders += order;
    return order;
}

void Track::initialize() {
}

void Track::reset() {
    _count = 1;
    _type = _port->type();
    _valuePtr = _port->valuePtr<void>();
    if (!_filteredValuePtr)
        _filteredValuePtr = port_value_op::allocate(_type);

    allocateBuffer();

    if (isFiltered())
       base::assign(_type, _filteredValuePtr, _type, _valuePtr, PortTransform::Identity, this);
    else
        _buffer.append(_valuePtr);
}

void Track::update() {
    ++_count;
    switch (_filter) {
        case PortFilter::None:
            _buffer.append(_valuePtr);
            break;
        case PortFilter::Sum:
        case PortFilter::Mean:
            port_value_op::accumulate(_type, _filteredValuePtr, _valuePtr);
            break;
        case PortFilter::Min:
            port_value_op::min(_type, _filteredValuePtr, _filteredValuePtr, _valuePtr);
            break;
        case PortFilter::Max:
            port_value_op::max(_type, _filteredValuePtr, _filteredValuePtr, _valuePtr);
        case PortFilter::End:
            base::assign(_type, _filteredValuePtr, _type, _valuePtr, PortTransform::Identity, this);
    }
}

void Track::cleanup() {
    if (_filter == PortFilter::Mean)
        port_value_op::divide(_type, _filteredValuePtr, _count);
    if (_filter != PortFilter::None)
        _buffer.append(_filteredValuePtr);
    checkBufferSizes();
}

void Track::checkBufferSizes() {
    bool hasTracks = !_tracks.isEmpty();
    if (hasTracks) {
        int n = _tracks.values().at(0)->_buffer.size();
        for (Track *track : _tracks.values()) {
            int m = track->_buffer.size();
            if (m != n)
                ThrowException("Unexpected error: buffer sizes do not match")
                        .value(n).value2(m).context(track);
        }
    }
}

int Track::bufferSize() {
    bool hasTracks = !_tracks.isEmpty();
    return hasTracks ? _tracks.values().at(0)->_buffer.size() : 0;
}

Port* Track::port() {
    return _port;
}

PortFilter Track::filter() const {
    return _filter;
}

void Track::uniqueName(QString name) {
    _uniqueName = name;
}

QString Track::uniqueName() const {
    return _uniqueName;
}

QStringList Track::uniqueNameExpanded() {
    // Expand name with running numbers if the port is a vector
    QStringList names;
    int n = _port->valueSize();
    if (n==1)
        names << _uniqueName;
    else {
        for (int i = 0; i < n; ++i)
            names << (_uniqueName + "_" + QString::number(i));
    }
    return names;
}

QString Track::toString(int row) {
    return _buffer.toString(row);
}

QList<Track *> Track::all() {
    return _tracks.values();
}

void Track::initializeAll() {
    _tracks.clear();
    for (Order order : _orders)
        _tracks[order] = new Track(order);
    _orders.clear();
    setUniqueNames();
}

void Track::setUniqueNames() {
    UniqueName unique(all());
    QStringList uniqueNames = unique.resolve();
    int i{0};
    for (Track *track : all())  {
        QString name = uniqueNames.at(i++);
        name.replace("/", ".");
        track->uniqueName(name);
    }
}

void Track::resetAll() {
    for (Track *track : all())
        track->reset();
}

void Track::updateAll() {
    for (Track *track : all())
        track->update();
}

void Track::cleanupAll() {
    for (Track *track : all())
        track->cleanup();
}


bool Track::isFiltered() const {
    return _filter != PortFilter::None;
}

Track* Track::find(Order order) {
    Tracks::iterator it = _tracks.find(order);
    if (it == _tracks.end())
        ThrowException("No track found matching order").
                value("Filter=" + convert<QString>(order.filter)).
                context(order.port);
    return it.value();
}


} //namespace


