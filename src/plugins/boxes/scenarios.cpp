#include <base/exception.h>
#include <base/publish.h>
#include "scenarios.h"

using namespace base;

namespace boxes {

PUBLISH(Scenarios)

Scenarios::Scenarios(QString name, QObject *parent)
    : Box(name, parent)
{
    help("runs scenarios from a text file");
    Input(fileName).help("Text file with columns, one for each output produced. One iteration is run for each line").equals("scenarios.txt");
    Output(atEnd).help("Has last iteration run?");
}

void Scenarios::amend() {
    ExceptionContext(this);
    readDataFrame();
    createColumnOutputs();
    atEnd = false;
    _ixRow = 0;
    copyValues();
}

void Scenarios::initialize() {
    ExceptionContext(this);
    // Set simulation to stop after last scenario
    Box *sim = findMaybeOne<Box>("/*<Simulation>");
    if (sim) {
        Port *useStopIterations = sim->port("useStopIterations"),
             *stopIterations = sim->port("stopIterations");
        if (!useStopIterations->value<bool>()) {
            useStopIterations->equals(true);
            stopIterations->imports(fullName() + "[atEnd]");
        }
    }
}

void Scenarios::readDataFrame() {
    _df.read(fileName, DataFrame::ColumnLabelled);
    if (_df.numRow() == 0)
        ThrowException("Data frame file is empty").value(fileName).context(this);
}

void Scenarios::createColumnOutputs() {
    values.fill(QString(), _df.numCol());
    for (QString colname : _df.colNames().toVector()) {
        Port *port = new Port(colname, this);
        int ixCol = _df.ixCol(colname);
        port->data(&values[ixCol]);
    }
}

void Scenarios::reset() {
    Q_ASSERT(!atEnd);
    copyValues();
    atEnd = (++_ixRow == _df.numRow());
}

void Scenarios::copyValues() {
    int i(0);
    for (QString value : _df.row(_ixRow))
        values[i++] = value;
}

} //namespace
