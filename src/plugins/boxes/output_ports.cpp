/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/path.h>
#include <base/port.h>
#include <base/track.h>
#include <base/track_collection.h>
#include "output_ports.h"

using namespace base;

namespace boxes {

OutputPorts::OutputPorts(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(OutputPorts);
    Input(ports).reference().help("Vector of ports to include in plot");

    _tracks = new TrackCollection("tracks", this);
}

void OutputPorts::amend() {
    Box *page = findMaybeOne<Box>("ancestors::*<PageR>");
    bool doPlaceOrders = !page ||
                         (page && page->port("show")->value<bool>());
    if (doPlaceOrders)
        placeOrders();
}

void OutputPorts::placeOrders() {
    Q_ASSERT(_orders.isEmpty());
    _orders = Track::placeOrders(ports, this);
}

const QVector<Track*> & OutputPorts::tracks() {
    if (_tracks->isEmpty()) {
        // Collect the tracks that match my orders
        for (Track::Order order : _orders)
            _tracks->append(Track::find(order));
    }
    return _tracks->tracks();
}

}
