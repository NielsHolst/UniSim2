#include <QFile>
//#include <QMapIterator>
#include "exception.h"
#include "data_frame.h"


namespace base {

DataFrame::DataFrame(QObject *parent)
    : QObject(parent)
{
}

void DataFrame::read(QString fileNamePath, QString sep) {

}

int DataFrame::ncol() const {
    return _rows.at(0).size();
}
int DataFrame::nrow() const {
    return _rows.size();
}
QStringList DataFrame::colnames() const {
    return QStringList(); //(_colnames.values());
}

int DataFrame::ixcol(QString colname) const {
    QMap<QString,int>::const_iterator it = _colnames.constFind(colname);
    if (it == _colnames.constEnd())
        ThrowException("No column with that name").value(colname).context(this);
    return it.value();
}

QStringList DataFrame::row(int i) const {
    return _rows.at(i+1);
}

QStringList DataFrame::col(int i) const {
    QStringList co;
}

QStringList DataFrame::col(QString colname) const {
    return col( ixcol(colname) );
}

} //namespace


