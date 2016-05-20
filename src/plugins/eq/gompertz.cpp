#include <algorithm>
#include <base/publish.h>
#include "gompertz.h"

using namespace base;

namespace eq {

PUBLISH(Gompertz)

Gompertz::Gompertz(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(x);
    Input(x0).equals(10);
    Input(y0).equals(0.05);
    Input(x1).equals(90);
    Input(y1).equals(0.95);
    Input(dx).equals(1);
    Input(yScale).equals(1);
    Output(y);
}

#define CHECK_Y(y) \
    if (y<=0 || y>1) \
        ThrowException(#y " must belong to the interval ]0;1]").context(this).value(y)

void Gompertz::reset() {
    CHECK_Y(y0);
    CHECK_Y(y1);
    y.fill(0, x.size());
    double A = log(-log(y0)),
           B = log(-log(y1));
    _b = (B-A)/(x0-x1),
    _a = exp(A + _b*x0);
}

inline double F(double x, double a, double b) {
    return exp(-a*exp(-b*x));
}

double Gompertz::f(double x) {
    double y0 = F(x, _a, _b),
           y1 = F(x+dx, _a, _b);
    return (1-y0 > 1e-30) ? yScale*(y1-y0)/(1-y0) : 0.;

}

void Gompertz::update() {
    Q_ASSERT(x.size() == y.size());
    std::transform(x.constBegin(), x.constEnd(), y.begin(), [this](const double &x){return this->f(x);});
}

}
