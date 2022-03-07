/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/port.h>
#include <base/publish.h>
#include "scenarios.h"

using namespace base;

namespace boxes {

PUBLISH(Scenarios)

Scenarios::Scenarios(QString name, Box *parent)
    : Box(name, parent)
{
    help("runs scenarios from a text file");
    Input(fileName).help("Text file with columns, one for each output produced. One iteration is run for each line").equals("scenarios.txt");
    Input(title).help("Name of column that holds title of scenario (optional)");
    Output(iterations).help("Number of iterations needed to run scenarios").noClear();
}

void Scenarios::amend() {
    ExceptionContext(this);
    readDataFrame();
    createColumnOutputs();
    _ixRow = 0;
    copyValues();
}

void Scenarios::initialize() {
    _ixRow = 0;
    iterations = _df.numRow();
}

void Scenarios::reset() {
    copyValues();
    ++_ixRow;
    if (!environment().isSilent()) {
        QString titleName = port("title")->value<QString>(),
                title = titleName.isEmpty() ? "" : port(titleName)->value<QString>();
    }
}

void Scenarios::readDataFrame() {
    _df.read(fileName, DataFrame::ColumnLabelled);
    if (_df.numRow() == 0)
        ThrowException("Data frame file is empty").value(fileName).context(this);
}

void Scenarios::createColumnOutputs() {
    values.fill(QString(), _df.numCol());
    for (QString colname : _df.colNames()) {
        int ixCol = _df.ixCol(colname);
        NamedOutput(colname, values[ixCol]).noClear();
    }
}

void Scenarios::copyValues() {
    int i(0);
    for (QString value : _df.row(_ixRow))
        values[i++] = value;
}

} //namespace
