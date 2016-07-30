#ifndef VECTORIZE_H
#define VECTORIZE_H
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
