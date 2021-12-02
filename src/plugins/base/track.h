/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_TRACK_H
#define BASE_TRACK_H

#include <QObject>
#include <QMap>
#include <QPair>
#include <QSet>

namespace base {

class Box;
class Port;

class Track : public QObject
{
public:
    typedef QVector<QString> ParseResult;

    struct Order {
        int portId;
        int key() const { return 100*portId; }
    };

    Track(Port *port);
    void reset();

    Port* port();
    bool isFiltered() const;
    void uniqueName(QString name);
    QString uniqueName() const;
    QStringList uniqueNameExpanded();

    static void clearOrders();
    static Order takeOrder(Port *port);
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
    QString _uniqueName;
    const void *_valuePtr;
    // Methods
    static bool areAnyOrdersFiltered();
    static void replaceUnfilteredOrders();
    static void setUniqueNames();

};



} //namespace

#endif

