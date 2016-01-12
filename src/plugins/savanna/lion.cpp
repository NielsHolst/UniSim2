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
    Input(growthRate).equals(1.5);
    Input(preyDensity).equals(0);
    Input(searchRate).equals(0.05);
    Input(demand).equals(0.01);
    Output(density);
    Output(killRate);
}

void Lion::reset() {
    density = initialDensity;
}

void Lion::update() {
    double supply = searchRate*preyDensity*density;
    if (supply > demand)
        supply = demand;
    if (supply > preyDensity)
        supply = preyDensity;
    double sd = (demand == 0.) ? 0 : supply/demand;
    killRate = (preyDensity ==0.) ? 0. : supply/preyDensity;
    density = sd*growthRate*density;
}

}
