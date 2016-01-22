#include "port_transform.h"

namespace base {

#define NAMEOF_TRANSFORM(X) \
    case X: \
        s = #X; \
        break

QString nameOf(PortTransform transform) {
    QString s;
    switch (transform) {
        NAMEOF_TRANSFORM(Identity);
        NAMEOF_TRANSFORM(Sum);
        NAMEOF_TRANSFORM(Average);
        NAMEOF_TRANSFORM(Min);
        NAMEOF_TRANSFORM(Max);
        NAMEOF_TRANSFORM(Copy);
        NAMEOF_TRANSFORM(Split);
        NAMEOF_TRANSFORM(All);
        NAMEOF_TRANSFORM(Any);
    }
    return s;
}

//#define LOOKUP_TRANSFORM(x) lookupTransform[#x] = x

//namespace {
//    QMap<QString, PortTransform> lookupTransform;
//    void init() {
//        LOOKUP_TRANSFORM(Identity);
//        LOOKUP_TRANSFORM(Sum);
//        LOOKUP_TRANSFORM(Average);
//        LOOKUP_TRANSFORM(Min);
//        LOOKUP_TRANSFORM(Max);
//        LOOKUP_TRANSFORM(Copy);
//        LOOKUP_TRANSFORM(Split);
//        LOOKUP_TRANSFORM(All);
//        LOOKUP_TRANSFORM(Any);
//    }
//}

}
