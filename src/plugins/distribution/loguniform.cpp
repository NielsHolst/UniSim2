#include <base/exception.h>
#include <base/publish.h>
#include "loguniform.h"

using namespace base;

namespace distribution {

PUBLISH(loguniform)

loguniform::loguniform(QString name, QObject *parent)
    : Distribution(name, parent), _min(this), _max(this)
{
}

void loguniform::min(double value) {
    _min.value = value;
    _min.transform();
}

void loguniform::max(double value) {
    _max.value = value;
    _max.transform();
}

void loguniform::parseArguments() {
    parseNext(&_min.value);
    parseNext(&_max.value);
    _min.transform();
    _max.transform();
}

QPair<double,double> loguniform::bounds() const {
    return qMakePair(0., 1.);
}

double loguniform::inverse(double y) const {
    Q_ASSERT(y>=0. && y<=1.);
    return exp(_min.tr + (_max.tr - _min.tr)*y);
}

}
