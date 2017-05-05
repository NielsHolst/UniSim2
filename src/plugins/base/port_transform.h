#ifndef BASE_PORT_TRANSFORM_H
#define BASE_PORT_TRANSFORM_H
#include <QMap>
#include <QString>
#include "convert.h"
#include "exception.h"

namespace base {

enum PortTransform {
    Identity,
    Sum,
    Prod,
    Mean,
    Min,
    Max,
    Copy,
    Split,
    All,
    Any
};

template<class T> T convert(PortTransform )  { ThrowException("Cannot only convert port transform to QString"); }

template<> QString convert(PortTransform transform);
template<> PortTransform convert(QString s);

QStringList portTransformNames();

}

#endif
