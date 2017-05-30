#include <QObject>
#include <base/exception.h>
#include "log_parameter.h"

using namespace base;

namespace distribution {

LogParameter::LogParameter(QObject *parent)
    : _parent(parent) {

}

void LogParameter::transform() {
    if (value<=0)
        ThrowException("Cannot log-transform parameter <= 0").value(value).context(_parent);
    tr = log(value);
}

}
