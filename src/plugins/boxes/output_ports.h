/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef OUTPUT_PORTS_H
#define OUTPUT_PORTS_H
#include <base/box.h>
#include <base/port_filter.h>

namespace base {
class Track;
class TrackCollection;
}

namespace boxes {

class OutputPorts : public base::Box
{
public:
    OutputPorts(QString name, QObject *parent);
    void amend() final;
    const QVector<base::Track*> & tracks();
    void parsePorts();
private:
    // Inputs
    QVector<QString> ports;
    // Data
    QVector<base::Track::Order> _orders;
    base::TrackCollection *_tracks;
    // Methods
    void placeOrders();
};

}

#endif
