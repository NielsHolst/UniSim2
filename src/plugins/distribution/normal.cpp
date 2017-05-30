#include <boost/math/distributions/normal.hpp>
#include <base/exception.h>
#include <base/publish.h>
#include "normal.h"

using namespace base;

namespace distribution {

PUBLISH(normal)

normal::normal(QString name, QObject *parent)
    : Distribution(name, parent),
      _mean(0), _sd(1),
      _lowerQuantile(0.01), _upperQuantile(0.99)
{
}

void normal::mean(double value) {
    _mean = value;
}

void normal::sd(double value) {
    _sd = value;
}

void normal::lowerQuantile(double value) {
    _lowerQuantile = value;
}

void normal::upperQuantile(double value) {
    _upperQuantile = value;
}

void normal::parseArguments() {
    parseNext(&_mean);
    parseNext(&_sd);
    parseNext(&_lowerQuantile);
    parseNext(&_upperQuantile);
}

QPair<double,double> normal::bounds() const {
    return qMakePair(_lowerQuantile, _upperQuantile);
}

double normal::inverse(double y) const {
    Q_ASSERT(y>=0. && y<=1.);
    boost::math::normal ndist(_mean, _sd);
    return quantile(ndist, y);
}

}
