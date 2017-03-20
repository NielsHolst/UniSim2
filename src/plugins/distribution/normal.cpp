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

double normal::draw() {
    if (_isFirstDraw) {
        _distribution = new RndDistribution(_mean, _sd);
        _variate = new Variate(*randomGenerator(), *_distribution);
        setBounds();
        _isFirstDraw = false;
    }
    int i = 0;
    double value;
    do {
        value = (*_variate)();
        if (++i > 30)
            ThrowException("Too many tries (30) finding value between quantiles")
                    .value(_lowerQuantile).value2(_upperQuantile)
                    .context(this);
    } while (value < _lowerBound || value >= _upperBound);
    return value;
}

void normal::setBounds() {
    boost::math::normal ndist(_mean, _sd);
    _lowerBound = quantile(ndist, _lowerQuantile);
    _upperBound = quantile(ndist, _upperQuantile);
}

}
