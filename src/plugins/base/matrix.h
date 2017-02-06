#ifndef BASE_MATRIX_H
#define BASE_MATRIX_H

#include <QVector>
#include "convert.h"
#include "data_frame.h"
#include "table.h"

namespace base {

template <class T>
class Matrix : public Table
{
public:
    Matrix(QObject *parent = 0);
    void read(QString fileName, Labelling labelling);
    int numRow() const;
    int numCol() const;
    QVector<T> row(int i) const;
    QVector<T> col(int i) const;
    T at(int row, int col) const;
    T& operator()(int row, int col);
    QVector<T> row(QString rowName) const;
    QVector<T> col(QString colName) const;
    QVector<QVector<T>> rows() const;
private:
    // Data
    DataFrame *_df;
    QVector<QVector<T>> _rowsTyped;
};

template <class T>
Matrix<T>::Matrix(QObject *parent)
    : Table(parent)
{
    setObjectName("Matrix");
    _df = new DataFrame(this);
}

template <class T>
void Matrix<T>::read(QString fileName, Labelling labelling) {
    _df->read(fileName, labelling);
    _rowsTyped.clear();
    for (int i=0; i<numRow(); ++i) {
        QStringList items = QStringList( _df->row<QString>(i).toList() );
        _rowsTyped << convert<T, QVector>(items);
    }
}

template <class T>
int Matrix<T>::numRow() const {
    return _df->numRow();
}

template <class T>
int Matrix<T>::numCol() const {
    return _df->numCol();
}

template <class T>
QVector<T> Matrix<T>::row(int i) const {
    return _rowsTyped.at(i);
}

template <class T>
QVector<T> Matrix<T>::col(int i) const {
    QVector<T> co;
    co << _rowsTyped.at(i);
    return co;
}

template <class T>
T Matrix<T>::at(int row, int col) {
    return _rowsTyped.at(row).at(col);
}

template <class T>
T& Matrix<T>::operator()(int row, int col) const {
    return _rowsTyped[row][col];
}

template <class T>
QVector<T> Matrix<T>::row(QString rowName) const {
    return row( ixRow(rowName) );
}

template <class T>
QVector<T> Matrix<T>::col(QString colName) const {
    return col( ixCol(colName) );
}

template <class T>
QVector<QVector<T>> Matrix<T>::rows() const {
    return _rowsTyped;
}


} //namespace

#endif

