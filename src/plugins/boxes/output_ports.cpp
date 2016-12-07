#include <base/path.h>
#include <base/port.h>
#include "output_ports.h"

using namespace base;

namespace boxes {

OutputPorts::OutputPorts(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(OutputPorts);
    Input(ports).help("Vector of ports to include in plot");
}

void OutputPorts::amend() {
    collectPorts();
    doAmend();
}

void OutputPorts::collectPorts() {
    // Check ports value
    checkPortsValue();
    // Collect only my ports
    _myPorts.clear();
    // Collect the ports
    for (QString portName : ports) {
        QVector<Port*> trackedPorts = resolveMany<Port>(portName);
        // Only add ports not already captured
        for (Port *port : trackedPorts) {
            if (_myPorts.contains(port))
                continue;
            // Ensure port will be tracked
            port->track();
            _myPorts << port;
        }
    }
    // Check if no ports given were actually found
    if (!ports.isEmpty() && _myPorts.isEmpty()) {
        QString shownValue{ "(" + QStringList(ports.toList()).join(" ") + ")" };
        ThrowException("No ports found").value(shownValue).context(this);
    }
}

void OutputPorts::checkPortsValue() {
    if (port("ports")->hasImport()) {
        ThrowException("Ports not found")
            .value(QStringList(ports.toList()).join(" "))
            .hint("Enclose the value in parentheses to make it a vector of paths")
            .id("PortsIsReference")
            .context(this);
    }
}

const QVector<base::Port*> & OutputPorts::myPorts() {
    return _myPorts;
}


}
