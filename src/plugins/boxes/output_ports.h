#ifndef OUTPUT_PORTS_H
#define OUTPUT_PORTS_H
#include <base/box.h>
#include "output_r.h"

namespace base {
    class Port;
}

namespace boxes {

class OutputPorts : public base::Box
{
public:
    OutputPorts(QString name, QObject *parent);
    void amend() final;
    virtual void doAmend(){}
    const QVector<base::Port*> & myPorts();
private:
    // Inputs
    QVector<QString> ports;
    // Data
    QVector<base::Port*> _myPorts;
    // Methods
    void collectPorts();
    void checkPortsValue();
};

}

#endif
