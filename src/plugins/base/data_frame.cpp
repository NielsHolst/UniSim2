#include <QFile>
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

void DataFrame::read(QString fileName) {
    QFile file;
    QString fileNamePath = environment().inputFileNamePath(fileName);
    file.setFileName(fileNamePath);
    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk)
        ThrowException("Cannot open file").value(fileNamePath).context(this);

    _rows.clear();
    _colnames.clear();
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
        if (_colnames.isEmpty()) {
            int c(0);
            for (QString name : items.toVector()) _colnames[name] = c++;
        }
        else {
            _rows << items;
        }
    }
}

int DataFrame::ncol() const {
    return (nrow() == 0) ? 0 : _rows.at(0).size();
}

int DataFrame::nrow() const {
    return _rows.size();
}

QStringList DataFrame::colnames() const {
    return QStringList(_colnames.keys());
}

int DataFrame::ixcol(QString colname) const {
    QMap<QString,int>::const_iterator it = _colnames.constFind(colname);
    if (it == _colnames.constEnd())
        ThrowException("No column with that name").value(colname).context(this)
                .hint("Valid names:\n" + QStringList(_colnames.keys()).join("\n"));
    return it.value();
}

QStringList DataFrame::row(int i) const {
    return _rows.at(i);
}

QStringList DataFrame::col(int i) const {
    if (i<0 || i>=ncol()) {
        QString msg("Column index out of bounds (0..%1)");
        ThrowException(msg.arg(ncol()-1)).value(i).context(this);
    }
    QStringList co;
    for (QStringList row : _rows) co << row.at(i);
    return co;
}

QStringList DataFrame::col(QString colname) const {
    return col( ixcol(colname) );
}

} //namespace


