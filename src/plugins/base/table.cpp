#include <QMapIterator>
#include "exception.h"
#include "table.h"

namespace base {

Table::Table(QObject *parent)
    : QObject(parent)
{
}

QStringList Table::colNames() const {
    return QStringList(_colNames.keys());
}

QStringList Table::rowNames() const {
    return QStringList(_rowNames.keys());
}

int Table::lookup(const QMap<QString,int> &names, QString name, QString direction) const {
    QMap<QString,int>::const_iterator it = names.constFind(name);
    if (it == names.constEnd())
        ThrowException("No " + direction + " with that name").value(name).context(this)
                .hint("Valid names:\n" + QStringList(names.keys()).join("\n"));
    return it.value();
}

int Table::ixRow(QString rowName) const {
    if (_rowNames.isEmpty())
        ThrowException("Table has no row names");
    return lookup(_rowNames, rowName, "ow");
}

int Table::ixCol(QString colName) const {
    if (_colNames.isEmpty())
        ThrowException("Table has no column names");
    return lookup(_colNames, colName, "column");
}

} //namespace


