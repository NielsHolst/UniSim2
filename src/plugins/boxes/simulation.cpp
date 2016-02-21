#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>
#include <base/unique_name.h>
#include "simulation.h"

using namespace base;

namespace boxes {

PUBLISH(Simulation)

Simulation::Simulation(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(Simulation);
    Input(iterations).equals(1);
    Input(steps).equals(1);
    Output(iteration).noReset().Rformat("factor").axis("facet");
    Output(step).axis("x");
}

void Simulation::initialize() {
    collectTrackedPorts();
    makePortLabelsUnique();
}

void Simulation::collectTrackedPorts() {
    _trackedPorts.clear();
    QVector<Port*> all = Path("*{Port}").resolve<Port>();
    for (Port *port : all) {
        if (port->trackPtr())
            _trackedPorts << port;
    }
}

void Simulation::makePortLabelsUnique() {
    UniqueName unique(_trackedPorts);
    QStringList labels = unique.resolve();
    int i{0};
    for (Port *port : _trackedPorts)  {
        QString label = labels.at(i++);
        label.replace("/", ".");
        port->label(label);
    }
}

void Simulation::run() {
    amendFamily();
    initializeFamily();
    for (iteration = 0; iteration < iterations; ++iteration) {
        resetFamily();
        for (step = 1; step <= steps; ++step)
            updateFamily();
        cleanupFamily();
    }
    debriefFamily();
}


}
