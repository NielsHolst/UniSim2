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
}

void OutputPorts::collectPorts() {
    if (ports.isEmpty())
        ThrowException("'ports' cannot be empty").context(this);
    // Collect the ports
    _ports.clear();
    for (QString portName : ports) {
        QVector<Port*> trackedPorts = resolveMany<Port>(portName);
        // Only add ports not already captured
        for (Port *port : trackedPorts) {
            if (_ports.contains(port))
                continue;
            // Ensure port will be tracked
            port->track();
            _ports << port;
        }
    }
    // Check for empty 'ports' or 'ports' referenced by mistake
    if (_ports.isEmpty()) {
        QString maybeHint, maybeId,
                shownValue{ QStringList(ports.toList()).join(" ") };
        if (port("ports")->hasImport()) {
            maybeHint = "Enclose the value in parentheses to make it a vector of strings";
            maybeId = "PortsIsReference";
        }
        else
            shownValue = "(" + shownValue + ")";

        ThrowException("Ports not found").value(shownValue).hint(maybeHint).id(maybeId);
    }
}

const QVector<base::Port*> & OutputPorts::trackedPorts() {
    return _ports;
}


}
