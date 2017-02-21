#include <base/exception.h>
#include <base/publish.h>
#include "maker.h"

using namespace base;

namespace boxes {

PUBLISH(Maker)

Maker::Maker(QString name, QObject *parent)
    : Box(name, parent)
{
    help("clones all boxes inside it");
    Input(replicates).equals(1).help("Number of clones to replicate from child boxes");
    Input(names).help("Names of clones; number of replicates set to number of names");
    Input(fileName).help("For every line in file, replicates will be cloned; column headings label inputs");
}

void Maker::amend() {
    // Read data frame if available
    _cloneFromFile = (!fileName.isEmpty());
    if (_cloneFromFile) {
        if (replicates > 1)
            ThrowException("Number of replicates cannot be set when fileName is also set").context(this);
        readDataFrame();
    }

    // Find the one child
    Box* child = findOne<Box>("./*");
    if (dynamic_cast<Maker*>(child))
        ThrowException("Maker inside Maker is now allowed").context(this);

    // Construct names of clones
    if (names.isEmpty()) {
        QString name = child->objectName();
        if (name.isEmpty()) name = "clone";
        for (int i=0; i< replicates; ++i)
            names << name + QString::number(i);
    }
    else if (replicates > 1) {
        ThrowException("Number of replicates cannot be set when names are also set").context(this);
    }
    else {
        replicates = names.size();
    }

    // Clone child then delete it
    int row(0);
    for (QString name : names) {
        if (_cloneFromFile) copyPortValuesFromColumns(row++);
        child->cloneFamily(name, this);
    }
    delete child;

    // Amend the clones
    QVector<Box*> clones = findMany<Box>("./*");
    for (Box *clone : clones)
        clone->amendFamily();
}

void Maker::readDataFrame() {
    // Read file
    _df.read(fileName, DataFrame::ColumnLabelled);
    if (_df.numRow() == 0)
        ThrowException("Data frame file is empty").value(fileName).context(this);

    // Get names or replicates
    QString namesCol = ".[names]";
    if (_df.colNames().contains(namesCol))
        names = _df.col(namesCol);
    else
        replicates = _df.numRow();

    // Set up column paths
    for (QString colName : _df.colNames()) {
        // Ensure column name path is local
        QString path = colName;
        if (path.startsWith("/") || path.startsWith(".."))
            ThrowException("Column headings cannot refer to ports outside Maker box").context(this);
        if (!path.startsWith("."))
            path.prepend("selfOrDescendants::");
        // And that it is found once
        findOne<Port>(path);
        // Leave at names column
        _columnPaths << ((colName == namesCol) ? QString() : path);
    }
}


void Maker::copyPortValuesFromColumns(int row) {
    int col(0);
    for (QString path : _columnPaths) {
        if (!path.isEmpty()) {
            Port *port = findOne<Port>(path);
            QString entry = _df.at(row,col);
            if (base::isPath(entry))
                port->imports(entry);
            else
                port->equals(entry);
        }
        ++col;
    }
}

} //namespace
