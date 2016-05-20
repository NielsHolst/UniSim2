#include <stdlib.h>
#include <algorithm>
#include <base/publish.h>
#include "zigmoid.h"

using namespace base;
using std::max;
using std::min;

namespace eq {

PUBLISH(Zigmoid)

Zigmoid::Zigmoid(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(xMin).equals(0);
    Input(xMax).equals(1);
    Input(yMax).equals(1);
    Input(x);
    Input(dx).equals(1);
    Output(y);
}

void Zigmoid::reset() {
    y.fill(0, x.size());
}

double Zigmoid::f(double x) {
    double x1, x2 = x + dx;
    if (x2 <= xMin)
        return 0.;
    if (x >= xMax)
        return yMax;
    x1 = max(x, xMin);
    x2 = min(x2, xMax);
    return (x2-x1)/(xMax-xMin)*yMax;
}

void Zigmoid::update() {
    std::transform(x.constBegin(), x.constEnd(), y.begin(), [this](const double &x){return this->f(x);});
}

}
