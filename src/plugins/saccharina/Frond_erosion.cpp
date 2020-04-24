#include <base/publish.h>
#include "Frond_erosion.h"

using namespace base;

namespace saccharina {

PUBLISH(Fronderosion)

Fronderosion::Fronderosion(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the apical frond loss (Eq 6)");
    Input(A).equals(8.0).help("Frond area, state variable");
    Input(eps).equals(0.22).help("Frond erosion parameter");
    Output(fronderosion).help("Frond erosion");
}

void Fronderosion::reset() {
   update();
}

void Fronderosion::update() {
    fronderosion = (pow(10,-6) * exp(eps*A))/(1 + pow(10,-6)*(exp(eps*A)-1));
}
}
