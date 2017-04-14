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
    struct Order {
        Port *port;
        PortFilter filter;
    };
    Track(Order order);
    ~Track();
    static Order takeOrder(Port *port, PortFilter filter=PortFilter::None);
    static void initializeAll();
    static void resetAll();
    static void updateAll();
    static void cleanupAll();
    void initialize();
    void reset();
    void update();
    void cleanup();
    Port* port();
    PortFilter filter() const;
    void uniqueName(QString name);
    QString uniqueName() const;
    QStringList uniqueNameExpanded();
    static QList<Track*> all();
    static int bufferSize();
    static Track* find(Order order);
    QString toString(int row);
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
    static void checkBufferSizes();
    static void setUniqueNames();

};



} //namespace

#endif

