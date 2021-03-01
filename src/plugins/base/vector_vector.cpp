/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QStringList>
#include "exception.h"
#include "vector.h"
#include "vector_vector.h"

namespace base {

VectorVector::VectorVector() {
}

void VectorVector::append(const Vector *vector) {
    vectors << vector;
}

int VectorVector::size() const {
    return vectors.size();
}

QString VectorVector::toString(int row, QString separator) const {
    QStringList sl;
    int N = vectors.size();
    if (N > 0) {
        checkSizes();
        Q_ASSERT(row < vectors.at(0)->size());
        for (int i = 0; i < N; ++i)
            sl << vectors.at(i)->toString(row, separator);
    }
    return sl.join(separator);
}

void VectorVector::checkSizes() const {
    int N = vectors.size();
    if (N < 2) return;
    QVector<int> sizes;
    int n{-1};
    bool ok {true};
    for (int i = 0; i < N; ++i) {
        int n2 = vectors.at(i)->size();
        ok = ok && (n==n2 || n==-1);
        sizes << n2;
    }
    if (!ok) {
        QStringList s;
        for (int i = 0; i < N; ++i)
            s << QString::number(sizes.at(i));
        QString msg{"Vectors are not of equal size: (%1)"};
        ThrowException(msg.arg(s.join(" ")));
    }
}

}
