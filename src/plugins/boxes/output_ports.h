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
    const QVector<base::Port*> & trackedPorts();
private:
    // Inputs
    QVector<QString> ports;
    // Data
    QVector<base::Port*> _ports;
    // Methods
    void collectPorts();
};

}

#endif
