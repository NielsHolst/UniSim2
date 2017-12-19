#include <base/publish.h>
#include "Capacity.h"

using namespace base;

namespace tuta {

PUBLISH(Capacity)

Capacity::Capacity(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(pestNumber).help("pest population size");
    Input(K).help("carrying capacity of system");
    Output(factor).help("controlling factor for pest fecundity");
}

void Capacity::reset() {
    factor = 1;
}

void Capacity::update() {
        factor = (K - pestNumber)/K;
}


}
