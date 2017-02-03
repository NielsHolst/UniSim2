#include <QFile>
#include <QFileInfo>
#include <QMapIterator>
#include "environment.h"
#include "exception.h"
#include "data_frame.h"
#include "general.h"


namespace base {

DataFrame::DataFrame(QObject *parent)
    : QObject(parent)
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
    _rowNames.clear();
    _colNames.clear();

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
            for (QString name : names.toVector()) _rowNames[name] = ixRow++;
        }

        if (hasColNames) {
            QStringList names = _rows.at(0);
            if (hasRowNames) names.removeFirst();
            int ixCol(0);
            for (QString name : names.toVector()) _colNames[name] = ixCol++;
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

QStringList DataFrame::colNames() const {
    return QStringList(_colNames.keys());
}

QStringList DataFrame::rowNames() const {
    return QStringList(_rowNames.keys());
}

int DataFrame::lookup(const QMap<QString,int> &names, QString name, QString direction) const {
    QMap<QString,int>::const_iterator it = names.constFind(name);
    if (it == names.constEnd())
        ThrowException("No " + direction + " with that name").value(name).context(this)
                .hint("Valid names:\n" + QStringList(names.keys()).join("\n"));
    return it.value();
}

int DataFrame::ixRow(QString rowName) const {
    if (_rowNames.isEmpty())
        ThrowException("Dataframe has no row names");
    return lookup(_rowNames, rowName, "ow");
}

int DataFrame::ixCol(QString colName) const {
    if (_colNames.isEmpty())
        ThrowException("Dataframe has no column names");
    return lookup(_colNames, colName, "column");
}

QStringList DataFrame::row(int i) const {
    return _rows.at(i);
}

QStringList DataFrame::col(int i) const {
    QStringList co;
    for (QStringList row : _rows) co << row.at(i);
    return co;
}

QString DataFrame::at(int row, int col) const {
    return _rows.at(row).at(col);
}

QString DataFrame::operator()(int row, int col) const {
    return at(row,col);
}

QStringList DataFrame::row(QString rowName) const {
    return row( ixRow(rowName) );
}

QStringList DataFrame::col(QString colName) const {
    return col( ixCol(colName) );
}

const QVector<QStringList>& DataFrame::rows() const {
    return _rows;
}

} //namespace


