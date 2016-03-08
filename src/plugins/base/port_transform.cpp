#include <QMap>
#include <QStringList>
#include "port_transform.h"

namespace base {

#define TRANSFORM_CASE(X) \
    case X: \
        s = #X; \
        break

#define TRANSFORM_ENTRY(X) \
    _map[#X] = X

namespace {
    const QMap<QString, PortTransform>& portTransformMap() {
        static QMap<QString, PortTransform> _map;
        if (_map.isEmpty()) {
            TRANSFORM_ENTRY(Identity);
            TRANSFORM_ENTRY(Sum);
            TRANSFORM_ENTRY(Average);
            TRANSFORM_ENTRY(Min);
            TRANSFORM_ENTRY(Max);
            TRANSFORM_ENTRY(Copy);
            TRANSFORM_ENTRY(Split);
            TRANSFORM_ENTRY(All);
            TRANSFORM_ENTRY(Any);
        }
        return _map;
    }
}

template<> QString convert(PortTransform transform) {
    QString s;
    switch (transform) {
        TRANSFORM_CASE(Identity);
        TRANSFORM_CASE(Sum);
        TRANSFORM_CASE(Average);
        TRANSFORM_CASE(Min);
        TRANSFORM_CASE(Max);
        TRANSFORM_CASE(Copy);
        TRANSFORM_CASE(Split);
        TRANSFORM_CASE(All);
        TRANSFORM_CASE(Any);
    }
    return s;
}

template<> PortTransform convert(QString s) {
    if (s.isEmpty())
        return Identity;
    if (portTransformMap().contains(s))
        return portTransformMap().value(s);
    QString msg{"Unknown transform.Must be one of '%1'"};
    throw Exception(msg.arg(portTransformNames().join(",")), s);
}

QStringList portTransformNames() {
    return QStringList(portTransformMap().keys());
}

}
