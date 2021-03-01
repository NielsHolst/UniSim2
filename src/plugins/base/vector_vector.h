/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_VECTOR_VECTOR_H
#define BASE_VECTOR_VECTOR_H
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
