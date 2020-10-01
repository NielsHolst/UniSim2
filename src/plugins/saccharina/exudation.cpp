#include <base/publish.h>
#include "exudation.h"

using namespace base;

namespace saccharina {

PUBLISH(Exudation)

Exudation::Exudation(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates carbon exudation rate (eq. 15)");
    Input(gamma).equals(0.5).help("Exudation parameter (gC g−1)");
    Input(C).imports("C[value]");
    Input(Cmin).imports("fcarbon[Cmin]");
    Output(value).help("Carbon exudation");
}

void Exudation::reset() {
   update();
}

void Exudation::update() {
    value = 1. - exp(gamma*(Cmin - C));
}
}
