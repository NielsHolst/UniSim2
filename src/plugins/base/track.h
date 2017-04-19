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

class Port;

class Track : public QObject
{
public:
    typedef QVector<QPair<QString, PortFilter>> ParseResult;

    struct Order {
        Port *port;
        PortFilter filter;
    };
    Track(Order order);
    ~Track();
    static void clearOrders();
    static Order takeOrder(Port *port, PortFilter filter=PortFilter::None);
    static void effectuateOrders();
    static void resetAll();
    static void updateAll();
    static void cleanupAll();
    void initialize();
    void reset();
    void update();
    void cleanup();
    Port* port();
    PortFilter filter() const;
    const Vector* buffer() const;
    void uniqueName(QString name);
    QString uniqueName() const;
    QStringList uniqueNameExpanded();
    QString toString(int row);

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
    Vector _buffer;
    const void *_valuePtr;
    void *_filteredValuePtr;
    int _count;
    bool _allocated;
    // Methods
    bool isFiltered() const;
    void allocateBuffer();
    static void setUniqueNames();

};



} //namespace

#endif

