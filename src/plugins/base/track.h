/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_TRACK_H
#define BASE_TRACK_H

#include <QObject>
#include <QMap>
#include <QSet>

namespace base {

class Box;
class Port;

class Track : public QObject
{
public:
    Track(const Port *port);
    void reset();
    const Port* port();
    void uniqueName(QString name);
    QString uniqueName() const;
    QStringList uniqueNameExpanded();

    static void clearOrders();
    static void placeOrders(QVector<const Port*> ports);
    static void effectuateOrders();

    static QList<Track *> all();
    static Track* find(int order);
private:
    // Object data
    const Port *_port;
    QString _uniqueName;
    const void *_valuePtr;

    // Static data
    using Orders = QMap<int, const Port*>;
    using Tracks = QMap<int, Track*>;

    static Orders _orders;
    static Tracks _tracks;

    // Methods
    static void setUniqueNames();

};



} //namespace

#endif

