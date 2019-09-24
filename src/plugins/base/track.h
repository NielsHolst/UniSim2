/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_TRACK_H
#define BASE_TRACK_H

#include <QObject>
#include <QMap>
#include <QPair>
#include <QSet>
#include "port_filter.h"
#include "port_type.h"
#include "vector.h"

namespace base {

class Box;
class Port;

class Track : public QObject
{
public:
    typedef QVector<QPair<QString, PortFilter>> ParseResult;

    struct Order {
        int portId;
        PortFilter filter;
        int key() const { return 100*portId + int(filter); }
    };

    Track(Port *port, PortFilter filter);
    void reset();

    Port* port();
    PortFilter filter() const;
    bool isFiltered() const;
    void uniqueName(QString name);
    QString uniqueName() const;
    QStringList uniqueNameExpanded();

    static void clearOrders();
    static Order takeOrder(Port *port, PortFilter filter=PortFilter::None);
    static void effectuateOrders();
    static void resetAll();

    static QList<Track*> all();
    static Track* find(Order order);
    static QString dumpOrders();
    static QString dumpTracks();
    static ParseResult parseTrackPaths(QVector<QString> paths, QObject *context = 0);
    static QVector<Order> placeOrders(QVector<QString> paths, Box *context);
private:
    // Static data
    typedef QMap<Order, Track*> Tracks;
    static QSet<Order> _orders;
    static Tracks _tracks;
    // Object data
    Port *_port;
    PortFilter _filter;
    QString _uniqueName;
    PortType _type;
    const void *_valuePtr;
    // Methods
    static bool areAnyOrdersFiltered();
    static void replaceUnfilteredOrders();
    static void setUniqueNames();

};



} //namespace

#endif

