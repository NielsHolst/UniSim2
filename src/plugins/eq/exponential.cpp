#include <stdlib.h>
#include <algorithm>
#include <base/publish.h>
#include "exponential.h"

using namespace base;
using std::max;
using std::min;

namespace eq {

PUBLISH(Exponential)

Exponential::Exponential(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(x);
    Input(r).equals(0);
    Input(dt).equals(1);
    Input(yMax).equals(1);
    Output(y);
}

void Exponential::reset() {
    y.fill(0, x.size());
}

double Exponential::f(double x) {
    double dx = x*(exp(r*dt)-1),
           dxMax = yMax - x;
    return (dx > dxMax) ? dxMax : dx;
}

void Exponential::update() {
    Q_ASSERT(x.size() == y.size());
    std::transform(x.constBegin(), x.constEnd(), y.begin(), [this](const double &x){return this->f(x);});
}
// line break in ports: ( \n  )
}
