#include <base/exception.h>
#include <base/publish.h>
#include "scenarios.h"

using namespace base;

namespace boxes {

PUBLISH(Scenarios)

Scenarios::Scenarios(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(fileName).equals("scenarios.txt");
    Output(atEnd);
}

void Scenarios::amend() {
    readDataFrame();
    createColumnOutputs();
    atEnd = false;
    _ixRow = 0;
    copyValues();
}

void Scenarios::readDataFrame() {
    _df.read(fileName, DataFrame::ColumnLabelled);
    if (_df.numRow() == 0)
        ThrowException("Data frame file is empty").value(fileName).context(this);
}

void Scenarios::createColumnOutputs() {
    values.fill(QString(), _df.numCol());
    int i(0);
    for (QString colname : _df.colNames().toVector()) {
        Port *port = new Port(colname, this);
        port->data(&values[i++]);
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
