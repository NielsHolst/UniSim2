#include <base/path.h>
#include <base/port.h>
#include <base/port_filter.h>
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
    doAmend();
}

void OutputPorts::collectOrders() {
    _orders.clear();
    for (QString portName : ports) {
        QVector<Port*> trackedPorts = findMany<Port>(portName);
        for (Port *port : trackedPorts) {
            _orders << port->track(PortFilter::None);
        }
    }
}

const QVector<Track *> &OutputPorts::tracks() {
    if (_tracks.isEmpty()) {
        for (Track::Order order : _orders)
            _tracks << Track::find(order);
    }
    return _tracks;
}


}
