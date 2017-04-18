#include <base/path.h>
#include <base/port.h>
#include "output_ports.h"

using namespace base;

namespace boxes {

OutputPorts::OutputPorts(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(OutputPorts);
    Input(ports).notReferenced().help("Vector of ports to include in plot");
}

void OutputPorts::amend() {
    collectOrders();
}

void OutputPorts::collectOrders() {
    Q_ASSERT(_orders.isEmpty());
    // Collect orders specified by my ports
    for (QString portPath : ports) {
        // For each path find the matching ports
        QVector<Port*> trackedPorts = findMany<Port>(portPath);
        // Register a track order for each port and keep own list of orders
        for (Port *port : trackedPorts) {
            _orders << port->track(PortFilter::None);
        }
    }
}

const QVector<Track*> & OutputPorts::tracks() {
    if (_tracks.isEmpty()) {
        // Collect the tracks the match my orders
        for (Track::Order order : _orders)
            _tracks << Track::find(order);
    }
    return _tracks;
}

void OutputPorts::parsePorts() {
    _parsedPorts.clear();
    int i=0, n=ports.size();
    QString port;
    PortFilter filter;
    while (i < n) {
        port = ports.at(i);
        filter = PortFilter::None;
        bool hasFilter = (i+1<n && ports.at(i+1)==">");
        if (hasFilter) {
            if (i+2==n)
                ThrowException("Trailing '>' in ports").value(port).context(this);
            filter = convert<PortFilter>(ports.at(i+2));
        }
        _parsedPorts << qMakePair(port, filter);
        i = hasFilter ? i+3 :i+1;
    }
}

QVector<QPair<QString,PortFilter>> OutputPorts::parsedPorts() {
    return _parsedPorts;
}

}
