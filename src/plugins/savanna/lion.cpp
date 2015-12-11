#include <boxes/port.h>
#include <boxes/publish.h>
#include "lion.h"

using namespace boxes;

namespace savanna {

PUBLISH(Lion)

Lion::Lion(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(Lion);
    Input(initialDensity).equals(0.01);
    Input(growthRate).equals(3.5);
    Input(carryingCapacity).equals(0.1);
    Output(density);
}

void Lion::reset() {
    density = initialDensity;
}

void Lion::update() {
    density = growthRate*density*(carryingCapacity-density/carryingCapacity);
}

}
