#include <base/publish.h>
#include "prey.h"

using namespace base;

namespace demo {

PUBLISH(Prey)

Prey::Prey(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(r).equals(20.).help("Slope of hyperbolic migration equation");
}

void Prey::initialize() {
}



}
