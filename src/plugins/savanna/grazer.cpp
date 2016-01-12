#include <boxes/port.h>
#include <boxes/publish.h>
#include "grazer.h"

using namespace boxes;

namespace savanna {

PUBLISH(Grazer)

Grazer::Grazer(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(Grazer);
    Input(initialDensity).equals(0.1);
    Input(growthRate).equals(4.5);
    Input(killRate).equals(0);
    Input(carryingCapacity).equals(0.1);
    Output(density);
}

void Grazer::reset() {
    density = initialDensity;
}

void Grazer::update() {
    density = (growthRate-killRate)*density*(carryingCapacity-density/carryingCapacity);
    if (density < 0.)
        density = 0.;
}

}
