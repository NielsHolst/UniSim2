#ifndef BASE_DATA_FRAME_H
#define BASE_DATA_FRAME_H

#include <QMap>
#include <QObject>
#include <QStringList>
#include <QVector>


namespace base {

class DataFrame : public QObject
{
public:
    enum Labelling {ColumnLabelled, RowLabelled, BothLabelled, NoLabelling};
    DataFrame(QObject *parent = 0);
    void read(QString fileName, Labelling labelling = ColumnLabelled);
    int numRow() const;
    int numCol() const;
    QStringList rowNames() const;
    QStringList colNames() const;
    int ixRow(QString rowName) const;
    int ixCol(QString colName) const;
    QStringList row(int i) const;
    QStringList col(int i) const;
    QString at(int row, int col) const;
    QString operator()(int row, int col) const;
    QStringList row(QString rowName) const;
    QStringList col(QString colName) const;
    const QVector<QStringList>& rows() const;
private:
    // Data
    QMap<QString,int> _rowNames, _colNames;
    QVector<QStringList> _rows;
    // Methods
    int lookup(const QMap<QString,int> &names, QString name, QString direction) const;
};


} //namespace

#endif

