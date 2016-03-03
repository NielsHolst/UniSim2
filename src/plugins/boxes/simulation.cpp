#include <base/environment.h>
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
    Output(iteration).noReset().Rformat("factor");
    Output(step).trackOff();
}

void Simulation::amend() {
    if (iterations == 1)
        port("iteration")->trackOff();
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
    UniqueName unique(_trackedPorts.toVector());
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

void Simulation::debrief() {
    openFileStream();
    writeDataFrame();
    _file.close();
    environment().incrementFileCounter();
}

void Simulation::openFileStream() {
    QString filePath = environment().outputFilePath(".txt");
    filePath.replace("\\", "/");
    _file.setFileName(filePath);
    if ( !_file.open(QIODevice::WriteOnly | QIODevice::Text) )
        throw Exception("Cannot open file for output", filePath, this);
    _stream.setDevice(&_file);
}

void Simulation::writeDataFrame() {
    // Check ports
    if (_trackedPorts.isEmpty())
        throw Exception("No ports are being tracked");

    removeObsoletePorts();

    // Check number of rows
    int nrow = _trackedPorts.at(0)->trackPtr()->size();
    for (Port *port : _trackedPorts) {
        Q_ASSERT(port->trackPtr()->size() == nrow);
    }
    Port *last = _trackedPorts.last();

    // Write column labels
    for (Port *port : _trackedPorts) {
        _stream << port->label();
        if (port != last)
            _stream << "\t";
    }
    _stream << "\n";

    // Write column format
    for (Port *port : _trackedPorts) {
        _stream << port->Rformat();
        if (port != last)
            _stream << "\t";
    }
    _stream << "\n";

    // Write column values as text
    for (int row = 0; row < nrow; ++row) {
        for (Port *port : _trackedPorts) {
            _stream << port->trackPtr()->toString(row);
            if (port != last)
                _stream << "\t";
        }
        if (row < nrow-1)
            _stream << "\n";
    }
}

void Simulation::removeObsoletePorts() {
    QList<Port*>::iterator itPort = _trackedPorts.begin();
    while(itPort != _trackedPorts.end()) {
        Port *port = *itPort;
        bool isFactor = port->Rformat() == "factor";
        bool isUninformative = valuesAreEqual(port);
        if (isFactor && isUninformative)
            itPort = _trackedPorts.erase(itPort);
        else
            itPort++;
    }
}

bool Simulation::valuesAreEqual(const Port *port) {
    const Vector *column = port->trackPtr();
    int nrow = column->size();
    if (nrow == 0)
        return true;
    QString value = column->toString(0);
    for (int row = 1; row < nrow; ++row) {
        if (column->toString(row) != value)
            return false;
    }
    return true;
}

}
