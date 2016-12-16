#include <base/convert.h>
#include <base/publish.h>
#include "normal.h"

using namespace base;

namespace distribution {

PUBLISH(normal)

normal::normal(QString name, QObject *parent)
    : Distribution(name, parent)
{
}

void normal::parseArguments(QStringList args) {
    mean = convert<double>(args.at(0));
    sd = convert<double>(args.at(1));
}

double normal::computeLevel(int , int ) const {
    return 0.;
}

}
