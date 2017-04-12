#ifndef BASE_PORT_FILTER_H
#define BASE_PORT_FILTER_H
#include <QMap>
#include <QString>
#include "convert.h"
#include "exception.h"

namespace base {

enum class PortFilter {
    None,
    Sum,
    Mean,
    Min,
    Max,
    End
};

template<class T> T convert(PortFilter)  { ThrowException("Cannot only convert port filter to QString"); }

template<> QString convert(PortFilter filter);
template<> PortFilter convert(QString s);

QStringList portFilterNames();

}

#endif
