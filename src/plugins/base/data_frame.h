#ifndef BASE_DATA_FRAME_H
#define BASE_DATA_FRAME_H

#include <QMap>
#include <QStringList>
#include <QVector>
#include "convert.h"


namespace base {

class DataFrame : public QObject
{
public:
    DataFrame(QObject *parent = 0);
    void read(QString fileName);
    int ncol() const;
    int nrow() const;
    QStringList colnames() const;
    int ixcol(QString colname) const;
    QStringList row(int i) const;
    QStringList col(int i) const;
    QStringList col(QString colname) const;
private:
    QVector<QStringList> _rows;
    QMap<QString,int> _colnames;
};


} //namespace

#endif

