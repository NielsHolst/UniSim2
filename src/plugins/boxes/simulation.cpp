#include <base/convert.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/path.h>
#include <base/port.h>
#include <base/port_type.h>
#include <base/publish.h>
#include <base/timer.h>
#include <base/unique_name.h>
#include "simulation.h"

using namespace base;

namespace boxes {

PUBLISH(Simulation)

Simulation::Simulation(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(iterations).equals(1);
    Input(steps).equals(1);
    Input(stopIterations).equals(false);
    Input(stopSteps).equals(false);
    Output(iteration).noReset();
    Output(step);
    Output(finalStep);
    Output(executionTime);
    Output(hasError);
    Output(errorMsg);
}

void Simulation::amend() {
    if (iterations > 1)
        port("iteration")->page("default");
}

void Simulation::initialize() {
    collectTrackedPorts();
    makePortLabelsUnique();
}

void Simulation::collectTrackedPorts() {
    _trackedPorts.clear();
    QVector<Port*> all = Path("*{Port}").resolveMany<Port>();
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
    dialog().message("Running...");
    nextShowProgress = 0.01;
    hasError = false;
    QTime time;
    try {
        time.start();
        dialog().information("initialize...");
        initializeFamily();
        for (iteration = 1; !stopIterations && iteration <= iterations; ++iteration) {
            dialog().information("reset...");
            resetFamily();
            dialog().information("update...");
            for (step = 1; !stopSteps && step <= steps; ++step) {
                show(time);
                updateFamily();
            }
            dialog().information("cleanup...");
            cleanupFamily();
        }
        dialog().information("debrief...");
        debriefFamily();
    }
    catch (Exception &ex) {
        hasError = true;
        errorMsg = ex.what();
    }
    dialog().finished();
    dialog().message("Ready");
    executionTime = time.elapsed();
}

void Simulation::show(QTime time) {
    double progress = double(step)/steps/iterations;
    if (progress > nextShowProgress) {
        double total = time.elapsed()/progress;
        dialog().progress(convert<int>(time.elapsed())/1000, convert<int>(total)/1000);
        nextShowProgress += 0.01;
    }
}

void Simulation::cleanup() {
    finalStep = step;
    step = 0;
}

void Simulation::debrief() {
    openFileStream(".txt");
    writeDataFrame();
    dialog().information("Data frame written to '" + environment().latestOutputFilePath("txt") + "'");
    _file.close();
    environment().incrementFileCounter();
}

void Simulation::openFileStream(QString extension) {
    environment().openOutputFile(_file, extension);
    _stream.setDevice(&_file);
}

void Simulation::writeDataFrame() {
    // Check ports
    if (_trackedPorts.isEmpty())
        ThrowException("No ports are being tracked");

    removeObsoletePorts();

    // Check number of rows
    int nrow = _trackedPorts.at(0)->trackPtr()->size();
    for (Port *port : _trackedPorts) {
        Q_ASSERT(port->trackPtr()->size() == nrow);
    }

    // Write column labels
    QStringList list;
    for (Port *port : _trackedPorts)
        list << port->labelList();
    _stream << list.join("\t") << "\n";

    // Write column format
    list.clear();
    for (Port *port : _trackedPorts) {
        for (int i = 0; i < port->valueSize(); ++i)
           list << port->format();
    }
    _stream << list.join("\t") << "\n";

    // Write column values as text
    Port *last = _trackedPorts.last();
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
        bool isFactor = port->format() == "factor";
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
