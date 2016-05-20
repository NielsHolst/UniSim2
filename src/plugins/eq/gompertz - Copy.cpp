#include <algorithm>
#include <base/publish.h>
#include "gompertz.h"

using namespace base;

namespace eq {

PUBLISH(Gompertz)

Gompertz::Gompertz(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(yMax).equals(1);
    Input(a).equals(1);
    Input(b).equals(1);
    Input(x);
    Input(dx).equals(1);
    Output(y);
}

void Gompertz::reset() {
    y.fill(0, x.size());
}

inline double F(double x, double a, double b) {
    return exp(-a*exp(-b*x));
}

double Gompertz::f(double x) {
    double result =  yMax*(F(x+dx, a, b) - F(x, a, b));
    return result;
}

void Gompertz::update() {
    std::transform(x.constBegin(), x.constEnd(), y.begin(), [this](const double &x){return this->f(x);});
}

}
