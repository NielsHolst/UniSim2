#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H
#include <QString>
#include <QVector>
#include "vector.h"

namespace base {

class Vector;

class VectorVector {
public:
    VectorVector();
    void append(const Vector *vector);
    int size() const;
    QString toString(int row, QString separator = "\t") const;
private:
    QVector<const Vector*> vectors;
    void checkSizes() const;
};

}
#endif
