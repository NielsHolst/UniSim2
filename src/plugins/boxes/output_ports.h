#ifndef OUTPUT_PORTS_H
#define OUTPUT_PORTS_H
#include <base/box.h>
#include <base/track.h>

namespace boxes {

class OutputPorts : public base::Box
{
public:
    OutputPorts(QString name, QObject *parent);
    void amend() final;
    virtual void doAmend(){}
    const QVector<base::Track*> & tracks();
private:
    // Inputs
    QVector<QString> ports;
    // Data
    QVector<base::Track::Order> _orders;
    QVector<base::Track*> _tracks;
    // Methods
    void collectOrders();
};

}

#endif
