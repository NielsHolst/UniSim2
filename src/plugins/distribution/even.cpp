#include <base/publish.h>
#include "even.h"

using namespace base;

namespace distribution {

PUBLISH(even)

even::even(QString name, QObject *parent)
    : Distribution(name, parent), _min(0), _max(1)
{
}

void even::min(double value) {
    _min = value;
}

void even::max(double value) {
    _max = value;
}

void even::parseArguments() {
    parseNext(&_min);
    parseNext(&_max);
}

QPair<double,double> even::bounds() const {
    return qMakePair(_min, _max);
}

double even::inverse(double y) const {
    Q_ASSERT(y>=0. && y<=1.);
    return _min + (_max - _min)*y;
}

}
