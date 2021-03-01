/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <boost/range/algorithm/random_shuffle.hpp>
#include "convert.h"
#include "exception.h"
#include "distribution.h"
#include "general.h"

namespace base {

Distribution::Distribution(QString name, QObject *parent)
    : QObject(parent), _isFirstDraw(true), _ixNext(0), _port(nullptr)
{
    Class(Distribution);
    setObjectName(name);
    uniform01 = new Uniform01(0., 1.);
    variate = new Variate(*randomGenerator(), *uniform01);
}

Distribution::~Distribution() {
    delete uniform01;
    delete variate;
}

void Distribution::mean(double value) {
    unavailable("mean", value);
}

void Distribution::sd(double value) {
    unavailable("sd", value);
}

void Distribution::min(double value) {
    unavailable("min", value);
}

void Distribution::max(double value) {
    unavailable("max", value);
}

void Distribution::lowerQuantile(double value) {
    unavailable("lowerQuantile", value);
}

void Distribution::upperQuantile(double value) {
    unavailable("upperQuantile", value);
}

void Distribution::unavailable(QString method, double value) {
    ThrowException("Input not available for this kind of distribution")
            .value(method).value(value);
}

void Distribution::arguments(QStringList args) {
    _args = args;
    parseArguments();
}

void Distribution::initialize(int numSamples) {
    if (numSamples <= 0)
        ThrowException("Number of samples must be > 0").value(numSamples).context(this);
    QPair<double,double> b = bounds();
    _y0 = b.first;
    _dy = (b.second - _y0)/numSamples;

    _ixNext = 0;

    _strata.clear();
    for (int i=0; i<numSamples; ++i)
        _strata << i;
    RandomGenerator::Shuffler s;
    boost::random_shuffle(_strata, s);
}

double Distribution::draw() {
    int n = _strata.size();
    if (!canDraw())
        Exception("Draw past size of distribution").value1(_ixNext).value2(n).context(this);
    int stratum = _strata.at(_ixNext);
    if (n > 1)
        ++_ixNext;
    double y = _y0 + _dy*(stratum + (*variate)());
    return inverse(y);
}

bool Distribution::canDraw() const {
    return (_ixNext < _strata.size());
}

void Distribution::port(Port *port_) {
    _port = port_;
}

Port* Distribution::port() {
    if (!_port)
        ThrowException("No parent port for distribution").
            value(objectName()).context(this);
    return _port;
}

void Distribution::parseNext(double *value) {
    if (_ixNext < _args.size())
        *value = convert<double>(_args.at(_ixNext++));
}

}
