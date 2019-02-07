#include <base/publish.h>
#include "Param_infection.h"

using namespace base;

namespace aphid {

PUBLISH(Param_infection)
// Do not consider temperature yet
Param_infection::Param_infection(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(LTimmigrants);
    Input(LTAdult);
    Input(LTNymph);
    Input(cadaverSurvival);

}

void Param_infection::reset() {

}

void Param_infection::update() {

}
}
