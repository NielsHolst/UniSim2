#include "port_transform.h"

namespace boxes {

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

}
