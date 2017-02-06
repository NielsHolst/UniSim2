#ifndef BASE_TABLE_H
#define BASE_TABLE_H

#include <QMap>
#include <QObject>
#include <QStringList>

namespace base {

class Table : public QObject
{
public:
    enum Labelling {ColumnLabelled, RowLabelled, BothLabelled, NoLabelling};
    Table (QObject *parent = 0);
    virtual void read(QString fileName, Labelling labelling) = 0;
    virtual int numRow() const = 0;
    virtual int numCol() const = 0;
    QStringList rowNames() const;
    QStringList colNames() const;
    int ixRow(QString rowName) const;
    int ixCol(QString colName) const;
protected:
    // Data
    QMap<QString,int> _rowNames, _colNames;
private:
    // Methods
    int lookup(const QMap<QString,int> &names, QString name, QString direction) const;
};


} //namespace

#endif

