#include <port.h>
#include "assign.h"
#include "box.h"
#include "environment.h"
#include "exception.h"
#include "port_value_op.h"
#include "track.h"
#include "unique_name.h"

#include <iostream>

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
    Box *parent = _port->boxParent();
    Q_ASSERT(parent);
    Port *iterations = parent->findMaybeOne<Port>("/*[iterations]"),
         *steps = parent->findMaybeOne<Port>("/*[steps]");
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
    QString s = QString::number(_orders.size()) + " " + port->fullName();
    std::cout << qPrintable(s) << "\n";
    Order order = Order{port, filter};
    _orders += order;
    return order;
}


void Track::effectuateOrders() {
    // If any order was filtered then all tracks must be filtered
    bool first = true,
         noFilter,
         mustFilter = false;
    for (Order order : _orders) {
        _tracks[order] = new Track(Order{order.port, order.filter});
        if (first) {
            noFilter = (order.filter==PortFilter::None);
            first = false;
        }
        else if (noFilter != (order.filter==PortFilter::None)) {
            mustFilter = true;
            break;
        }
    }
    // Set missing files to End
    if (mustFilter) {
        QSet<Order> oldOrders = _orders;
        _orders.clear();
        for (Order order : oldOrders) {
            if (order.filter==PortFilter::None)
                _orders << Order{order.port, PortFilter::End};
            else
                _orders << order;
        }
    }

    // Turn orders into tracks
    _tracks.clear();
    bool hasFilter = false;
    for (Order order : _orders) {
        _tracks[order] = new Track(Order{order.port, order.filter});
        hasFilter = hasFilter || order.filter!=PortFilter::None;
    }
    // If any order was filtered then all tracks must be filtered;
    // the orders are not changed in any case
    if (hasFilter) {
        QMapIterator<Order, Track*> it(_tracks);
        while (it.hasNext()) {
            it.next();
            Track *track = it.value();
            if (track->_filter == PortFilter::None)
                track->_filter = PortFilter::End;
        }
    }
    // Set unique name of each track
    setUniqueNames();
}

void Track::initialize() {
}

void Track::reset() {
    _count = 1;
    _type = _port->type();
    _valuePtr = _port->valuePtr<void>();
    if (isFiltered()){
        if (isVector(_type))
            ThrowException("You cannot apply a filter to a vector port output")
                    .value(convert<QString>(_filter)).context(_port);
        if (!_filteredValuePtr)
            _filteredValuePtr = port_value_op::allocate(_type);
    }

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
}

Port* Track::port() {
    return _port;
}

PortFilter Track::filter() const {
    return _filter;
}

const Vector* Track::buffer() const {
    return &_buffer;
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

void Track::clearOrders() {
    _orders.clear();
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
    // If not found then ttry with None filter replaced by End
    Tracks::iterator it = _tracks.find(order);
    if (it == _tracks.end() && order.filter==PortFilter::None)
        it = _tracks.find(Order{order.port, PortFilter::End});
    if (it == _tracks.end())
        ThrowException("No track found matching order").
                value("Filter=" + convert<QString>(order.filter)).
                context(order.port);
    return it.value();
}

Track::ParseResult Track::parseTrackPaths(QVector<QString> paths_, QObject *context) {
    // Break ">" loose as separate strings
    QVector<QString> paths;
    for (QString s : paths_) {
        if (s == ">")
            paths.append(">");
        else if (s.startsWith(">")) {
            paths.append(">");
            paths.append(s.remove(0, 1));
        }
        else if (s.endsWith(">")) {
            paths.append(s.remove(s.size()-1, 1));
            paths.append(">");
        }
        else if (s.contains(">")) {
            int ix = s.indexOf(">");
            paths.append(s.mid(0, ix));
            paths.append(">");
            paths.append(s.mid(ix+1));
        }
        else
            paths.append(s);
    }
    // Parse
    Track::ParseResult parsed;
    int i=0, n=paths.size();
    QString port;
    PortFilter filter;
    while (i < n) {
        port = paths.at(i);
        filter = PortFilter::None;
        bool hasFilter = (i+1<n && paths.at(i+1)==">");
        if (port == ">")
            ThrowException("Leading '>' in ports").value(port).context(context);
        if (hasFilter) {
            if (i+2==n)
                ThrowException("Trailing '>' in ports").value(port).context(context);
            filter = convert<PortFilter>(paths.at(i+2));
        }
        parsed << qMakePair(port, filter);
        i = hasFilter ? i+3 :i+1;
    }
    return parsed;
}

QVector<Track::Order> Track::placeOrders(QVector<QString> paths, Box *context) {
    // Parse paths into ports and filters
    Track::ParseResult pathAndFilter = Track::parseTrackPaths(paths, context);

    // Place orders specified by ports
    QVector<Order> orders;
    for (QPair<QString, base::PortFilter> pf : pathAndFilter) {
        // For each path find the matching ports
        QVector<Port*> trackedPorts = context->findMany<Port>(pf.first);
        // Register a track order for each port and keep own list of orders
        for (Port *port : trackedPorts) {
            orders << port->track(pf.second);
        }
    }

    // Return orders placed
    return orders;
}

QString Track::dumpOrders() {
    QStringList list;
    int i=0;
    for (Order order : _orders) {
        QString s = QString::number(i++) +
                "set[" + order.port->fullName() + " " + convert<QString>(order.filter) + "]";
        list << s;
    }
    return list.join("\n") + "\n";
}

QString Track::dumpTracks() {
    QStringList list;
    QMapIterator<Order, Track*> it(_tracks);
    int i=0;
    while (it.hasNext()) {
        it.next();
        Order order = it.key();
        Track *track = it.value();
        QString s = QString::number(i++) +
                " map[" + order.port->fullName() + " " + convert<QString>(order.filter) + "] = " +
                track->uniqueName();
        list << s;
    }
    return list.join("\n") + "\n";
}


} //namespace


