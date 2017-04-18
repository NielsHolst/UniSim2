#ifndef OUTPUT_PORTS_H
#define OUTPUT_PORTS_H
#include <base/box.h>
#include <base/port_filter.h>
#include <base/track.h>

namespace boxes {

class OutputPorts : public base::Box
{
public:
    OutputPorts(QString name, QObject *parent);
    void amend() final;
    const QVector<base::Track*> & tracks();
    void parsePorts();
    QVector<QPair<QString, base::PortFilter>> parsedPorts();
private:
    // Inputs
    QVector<QString> ports;
    // Data
    QVector<base::Track::Order> _orders;
    QVector<base::Track*> _tracks;
    QVector<QPair<QString, base::PortFilter>> _parsedPorts;
    // Methods
    void collectOrders();
};

}

#endif
