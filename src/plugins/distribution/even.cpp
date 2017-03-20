#include <base/publish.h>
#include <base/random_generator.h>
#include <base/test_num.h>
#include "even.h"

using namespace base;

namespace distribution {

PUBLISH(even)

even::even(QString name, QObject *parent)
    : Distribution(name, parent), _min(0), _max(1),
      _distribution(0), _variate(0)
{
}

even::~even() {
    delete _distribution;
    delete _variate;
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

//double even::computeLevel(int levelNumber, int noOfLevels) const {
//    return _min + double(levelNumber)/noOfLevels*(_max-_min);
//}

double even::draw() {
    if (_isFirstDraw) {
        if (_min > _max) {
            QString val{"%1 > %2"};
            ThrowException("Minimum value cannot be larger than maximum value")
                .value(val.arg(_min).arg(_max))
                .hint("Swap values")
                .context(this);
        }
        _distribution = new RndDistribution(_min, _max);
        _variate = new Variate(*randomGenerator(), *_distribution);
        _isFirstDraw = false;
    }
    return (*_variate)();
}

}
