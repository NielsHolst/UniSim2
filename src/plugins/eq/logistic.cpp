#include <base/exception.h>
#include <base/publish.h>
#include "logistic.h"

using namespace base;

namespace eq {

PUBLISH(Logistic)

Logistic::Logistic(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes logistic growth in population density");
    Input(x).help("Population density");
    Input(r).help("Intrinsic rate of increase");
    Input(K).help("Carrying capacity");
    Input(timeStep).equals(1).help("Carrying capacity");
    Output(value).help("Population increment");
}

void Logistic::update() {
    double &y0(x);
    double l = exp(r*timeStep);
    double y = K*y0*l/(K + y0*(l-1));
    value = y - y0;
}

} //namespace

