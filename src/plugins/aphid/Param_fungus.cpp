#include <base/publish.h>
#include "Param_fungus.h"

using namespace base;

namespace aphid {

PUBLISH(Param_fungus)
// Do not consider temperature yet
Param_fungus::Param_fungus(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(T0Fungus);
    Input(TmaxFungus);
    Input(ToptFungus);
    Input(Threshold_RH);
    Input(virulence)
}

void Param_fungus::reset() {

}

void Param_fungus::update() {

}
}
