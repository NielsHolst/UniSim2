#include <iostream>
#include <QtAlgorithms>
#include <QDir>
#include <QTime>
#include <base/convert.h>
#include <base/data_frame.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/publish.h>
#include "scenarios.h"

using namespace base;

namespace boxes {

PUBLISH(Scenarios)

Scenarios::Scenarios(QString name, QObject *parent)
    : Box(name, parent),
      _df(new DataFrame(this))
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
    QString fileNamePath = environment().inputFileNamePath(fileName);
    _df->read(fileNamePath);
    if (_df->nrow() == 0)
        ThrowException("Data frame file is empty").value(fileName).context(this);
}

void Scenarios::createColumnOutputs() {
    values.fill(QString(), _df->ncol());
    int i(0);
    for (QString colname : _df->colnames().toVector()) {
        Port *port = new Port(colname, this);
        port->data(&values[i++]);
    }
}

void Scenarios::reset() {
    Q_ASSERT(!atEnd);
    copyValues();
    atEnd = (++_ixRow == _df->nrow());
}

void Scenarios::copyValues() {
    int i(0);
    for (QString value : _df->row(_ixRow))
        values[i++] = value;
}

} //namespace
