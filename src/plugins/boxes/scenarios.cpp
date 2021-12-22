/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/dialog.h>
#include <base/environment.h>
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
    Input(title).help("Name of column that holds title of scenario (optional)");
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

void Scenarios::reset() {
    Q_ASSERT(!atEnd);
    copyValues();
    atEnd = (++_ixRow == _df.numRow());

    if (!environment().isSilent()) {
        QString titleName = port("title")->value<QString>(),
                title = titleName.isEmpty() ? "" : port(titleName)->value<QString>();

        dialog().information(QString::number(_ixRow) + " " + title);
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
        port->equals(&values[ixCol]);
    }
}

void Scenarios::copyValues() {
    int i(0);
    for (QString value : _df.row(_ixRow))
        values[i++] = value;
}

} //namespace
