#include <base/publish.h>
#include "exponential_peak.h"

using namespace base;

namespace cmbp {

PUBLISH(ExponentialPeak)

ExponentialPeak::ExponentialPeak(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(d1).help("Regression parameter");
    Input(d2).help("Regression parameter");
    Input(x).help("Independent variable");
    Input(optimum).help("x-value at peak y");
    Output(value).help("Peak curve");
}

void ExponentialPeak::reset() {
    update();
}

void ExponentialPeak::update() {
    value = exp(-0.5*pow( fabs(x-optimum)/d1, d2));
}


}
