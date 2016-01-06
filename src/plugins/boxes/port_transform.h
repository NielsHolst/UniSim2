#ifndef PORT_TRANSFORM_H
#define PORT_TRANSFORM_H
#include <QString>

namespace boxes {

enum PortTransform {
    Identity,
    Sum,
    Average,
    Min,
    Max,
    Copy,
    Split,
    All,
    Any
};

QString nameOf(PortTransform transform);

}

#endif
