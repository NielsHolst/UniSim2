/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "environment.h"
#include "exception.h"
#include "port.h"
#include "track.h"
#include "unique_name.h"
#include "value.h"

namespace base {

//
// Global
//

Track::Orders Track::_orders;
Track::Tracks Track::_tracks;

//
// Track
//

Track::Track(const Port *port)
    : QObject(const_cast<Port*>(port)),
        _port(port)
{
}

void Track::clearOrders() {
    _orders.clear();
}

void Track::effectuateOrders() {
    // Turn orders into tracks
    _tracks.clear();
    // Create tracks
    for (auto it = _orders.cbegin(); it!=_orders.cend(); ++it)
        _tracks[it.key()] = new Track(it.value());
    // Set unique name of each track
    setUniqueNames();
}

const Port* Track::port() {
    return _port;
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
    int n = _port->value<Value>().size();
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

void Track::placeOrders(QVector<const Port*> ports) {
    // Register an order for each port
    // If same port is ordered again, then same entry will just be overwritten with the same port
    for (const Port *port : ports)
        _orders[ port->evaluationOrder() ] = port;
}


} //namespace


