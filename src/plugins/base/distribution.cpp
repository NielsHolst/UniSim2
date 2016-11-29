#include "distribution.h"

namespace base {

Distribution::Distribution(QString name, QObject *parent)
    : QObject(parent)
{
    setObjectName(name);
}

void Distribution::arguments(QStringList args) {
    _args = args;
}

QStringList Distribution::arguments() const {
    return _args;
}


}
