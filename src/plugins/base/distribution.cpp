#include "convert.h"
#include "exception.h"
#include "distribution.h"

namespace base {

Distribution::Distribution(QString name, QObject *parent)
    : QObject(parent), _isFirstDraw(true)
{
    setObjectName(name);
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
    _ixNext = 0;
    parseArguments();
}

void Distribution::port(Port *port_) {
    _port = port_;
}

Port* Distribution::port() {
    return _port;
}

void Distribution::parseNext(double *value) {
    if (_ixNext < _args.size())
        *value = convert<double>(_args.at(_ixNext++));
}

//double Distribution::level(int levelNumber, int noOfLevels) {
//    if (levelNumber<0 || levelNumber>noOfLevels)
//        ThrowException("Level must be inside [0;noOfLevels]").value(levelNumber).context(this);
//    return computeLevel(levelNumber, noOfLevels);
//}


}
