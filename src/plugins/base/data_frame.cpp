#include <QFile>
#include <QFileInfo>
#include <QMapIterator>
#include "environment.h"
#include "exception.h"
#include "data_frame.h"
#include "general.h"


namespace base {

DataFrame::DataFrame(QObject *parent)
    : Table(parent)
{
    setObjectName("DataFrame");
}

void DataFrame::read(QString fileName, Labelling labelling) {
    QFile file;
    QString fileNamePath = QFileInfo(fileName).isAbsolute() ?
                fileName :
                environment().inputFileNamePath(fileName);

    file.setFileName(fileNamePath);
    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk)
        ThrowException("Cannot open file").value(fileNamePath).context(this);

    bool hasRowNames = (labelling==RowLabelled || labelling==BothLabelled),
         hasColNames = (labelling==ColumnLabelled || labelling==BothLabelled);

    _rows.clear();
    _rowNamesLookup.clear();
    _colNamesLookup.clear();

    QString line;
    int n = -1;
    while (true) {
        line.clear();
        while (!file.atEnd() && line.isEmpty()) {
            line = QString(file.readLine().simplified());
        }
        if (file.atEnd() && line.isEmpty()) break;
        QStringList items = base::split(line, this);
        int n2 = items.size();
        if (n>-1 && n!=n2) {
            QString msg{"Different number of items in this line (%1) compared to previous line (%2)"};
            ThrowException(msg.arg(n2).arg(n)).value(line).context(this);
        }
        n = n2;
        _rows << items;
    }

    if (!_rows.isEmpty()) {
        if (hasRowNames) {
            QStringList names;
            for (QStringList row : _rows.toList()) names << row.at(0);
            if (hasColNames) names.removeFirst();
            int ixRow(0);
            _rowNames.clear();
            for (QString name : names.toVector()) {
                _rowNames += name;
                _rowNamesLookup[name] = ixRow++;
            }
        }

        if (hasColNames) {
            QStringList names = _rows.at(0);
            if (hasRowNames) names.removeFirst();
            int ixCol(0);
            _colNames.clear();
            for (QString name : names.toVector()) {
                _colNames += name;
                _colNamesLookup[name] = ixCol++;
            }
        }

        if (hasColNames) _rows.removeFirst();

        if (hasRowNames) {
            int n = _rows.size();
            for (int i=0; i<n; ++i)
                _rows[i].removeFirst();
        }
    }
}

int DataFrame::numCol() const {
    return (numRow() == 0) ? 0 : _rows.at(0).size();
}

int DataFrame::numRow() const {
    return _rows.size();
}

} //namespace


