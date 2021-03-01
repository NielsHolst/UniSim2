/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_VECTORIZE_H
#define BASE_VECTORIZE_H
#include <QVector>
#include "assign.h"
#include "port_type.h"

namespace base {
    class Port;

    const void* vectorize(PortType destType, const QVector<Port*> &ports);

    template <class T> const void* vectorize(const QVector<Port*> &ports) {
        static QVector<T> typedVector;
        typedVector.resize(ports.size());
        T *destPtr = typedVector.data();
        for (const Port *source : ports) {
            assign(typeOf<T>(), destPtr, asScalar(source->type()), source->valuePtr<void>(), PortTransform::Identity, source);
            ++destPtr;
        }
        return &typedVector;
    }

}

#endif
