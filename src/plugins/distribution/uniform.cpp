#include <base/publish.h>
#include "uniform.h"

using namespace base;

namespace distribution {

PUBLISH(uniform)

uniform::uniform(QString name, QObject *parent)
    : Distribution(name, parent), _min(0), _max(1)
{
}

void uniform::min(double value) {
    _min = value;
}

void uniform::max(double value) {
    _max = value;
}

void uniform::parseArguments() {
    parseNext(&_min);
    parseNext(&_max);
}

QPair<double,double> uniform::bounds() const {
    return qMakePair(_min, _max);
}

double uniform::inverse(double y) const {
    Q_ASSERT(y>=0. && y<=1.);
    return _min + (_max - _min)*y;
}

}
