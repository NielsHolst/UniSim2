#include <base/publish.h>
#include "Carbon_exudation.h"

using namespace base;

namespace saccharina {

PUBLISH(Carbonexudation)

Carbonexudation::Carbonexudation(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the carbon exudation (Eq 15)");
    Input(gamma).equals(0.5).help("Exudation parameter (gC g−1)");
    Input(C).equals(0.01).help("Carbon reserves, relative to Ws, state variable (gC (gsw)−1)");
    Input(Cmin).equals(0.01).help("Minimal carbon reserve");
    Output(carbonexudation).help("Carbon exudation");
}

void Carbonexudation::reset() {
   update();
}

void Carbonexudation::update() {
    carbonexudation = 1 - exp(gamma * (Cmin - C));
}
}
