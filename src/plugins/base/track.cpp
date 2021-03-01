/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "assign.h"
#include "box.h"
#include "environment.h"
#include "exception.h"
#include "port.h"
#include "port_value_op.h"
#include "track.h"
#include "unique_name.h"

namespace base {

//
// Global
//

QSet<Track::Order> Track::_orders;
Track::Tracks Track::_tracks;

//
// Order
//

bool operator==(const Track::Order &a, const Track::Order &b) {
    return a.key() == b.key();
}

bool operator<(const Track::Order &a, const Track::Order &b) {
    return a.key() < b.key();
}

uint qHash(Track::Order order) {
    return order.key();
}

//
// Track
//

Track::Track(Port *port, PortFilter filter)
    : QObject(port),
      _port(port), _filter(filter)
{
}

Track::Order Track::takeOrder(Port *port, PortFilter filter) {
    Order order = Order{port->id(), filter};
    _orders += order;
    return order;
}


void Track::effectuateOrders() {
    // Turn orders into tracks;
    // if any order was filtered then all tracks must be filtered;
    // set missing filters to End
    if (areAnyOrdersFiltered())
        replaceUnfilteredOrders();
    _tracks.clear();
    // Create tracks
    for (Order order : _orders)
        _tracks[order] = new Track(Port::find(order.portId), order.filter);
    // Set unique name of each track
    setUniqueNames();
}

bool Track::areAnyOrdersFiltered() {
    bool status, first = true, mustFilter = false;
    for (Order order : _orders) {
        if (first) {
            status = (order.filter == PortFilter::None);
            first = false;
        }
        else if (status != (order.filter==PortFilter::None)) {
            mustFilter = true;
            break;
        }
    }
    return mustFilter;
}

void Track::replaceUnfilteredOrders() {
    QSet<Order> oldOrders = _orders;
    _orders.clear();
    for (Order order : oldOrders) {
        bool isUnfiltered = (order.filter == PortFilter::None);
        _orders << (isUnfiltered ? Order{order.portId, PortFilter::End} : order);
    }
}

void Track::reset() {
    _type = _port->type();
    _valuePtr = _port->valuePtr<void>();
    if (isFiltered()){
        if (isVector(_type))
            ThrowException("You cannot apply a filter to a vector port output")
                    .value(convert<QString>(_filter)).context(_port);
    }
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

bool Track::isFiltered() const {
    return _filter != PortFilter::None;
}

Track* Track::find(Order order) {
    // If not found then try with None filter replaced by End
    Tracks::iterator it = _tracks.find(order);
    if (it == _tracks.end() && order.filter==PortFilter::None)
        it = _tracks.find(Order{order.portId, PortFilter::End});
    if (it == _tracks.end()) {
        ThrowException("Unexpected. No track found matching order").
                value(Port::find(order.portId)->fullName()).
                value2(convert<QString>(order.filter));
    }
    return it.value();
}

Track::ParseResult Track::parseTrackPaths(QVector<QString> paths_, QObject *context) {
    // Break "|" loose as separate strings
    QVector<QString> paths;
    for (QString s : paths_) {
        if (s == "|")
            paths.append("|");
        else if (s.startsWith("|")) {
            paths.append("|");
            paths.append(s.remove(0, 1));
        }
        else if (s.endsWith("|")) {
            paths.append(s.remove(s.size()-1, 1));
            paths.append("|");
        }
        else if (s.contains("|")) {
            int ix = s.indexOf("|");
            paths.append(s.mid(0, ix));
            paths.append("|");
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
        bool hasFilter = (i+1<n && paths.at(i+1)=="|");
        if (port == "|")
            ThrowException("Leading '|' in ports").value(port).context(context);
        if (hasFilter) {
            if (i+2==n)
                ThrowException("Trailing '|' in ports").value(port).context(context);
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
                "set[" + QString::number(order.portId) + " " + convert<QString>(order.filter) + "]";
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
                " map[" + QString::number(order.portId) + "." + convert<QString>(order.filter) + "] = " +
                track->uniqueName();
        list << s;
    }
    return list.join("\n") + "\n";
}


} //namespace


