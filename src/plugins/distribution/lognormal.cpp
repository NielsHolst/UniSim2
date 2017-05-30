#include <boost/math/distributions/normal.hpp>
#include <base/exception.h>
#include <base/publish.h>
#include "lognormal.h"

using namespace base;

namespace distribution {

PUBLISH(lognormal)

lognormal::lognormal(QString name, QObject *parent)
    : Distribution(name, parent),
      _mean(this), _sd(1),
      _lowerQuantile(0.01), _upperQuantile(0.99)
{
}

void lognormal::mean(double value) {
    _mean.value = value;
    _mean.transform();
}

void lognormal::sd(double value) {
    _sd = value;
}

void lognormal::lowerQuantile(double value) {
    _lowerQuantile = value;
}

void lognormal::upperQuantile(double value) {
    _upperQuantile = value;
}

void lognormal::parseArguments() {
    parseNext(&_mean.value);
    parseNext(&_sd);
    parseNext(&_lowerQuantile);
    parseNext(&_upperQuantile);
    _mean.transform();
}

QPair<double,double> lognormal::bounds() const {
    return qMakePair(_lowerQuantile, _upperQuantile);
}

double lognormal::inverse(double y) const {
    Q_ASSERT(y>=0. && y<=1.);
    boost::math::normal ndist(_mean.tr, _sd);
    return exp(quantile(ndist, y));
}

}
