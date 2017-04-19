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
    placeOrders();
}

void OutputPorts::placeOrders() {
    Q_ASSERT(_orders.isEmpty());
    _orders = Track::placeOrders(ports, this);
}

const QVector<Track*> & OutputPorts::tracks() {
    if (_tracks.isEmpty()) {
        // Collect the tracks that match my orders
        for (Track::Order order : _orders)
            _tracks << Track::find(order);
    }
    return _tracks;
}

}
