#include <base/publish.h>
#include "Param_aphid.h"

using namespace base;

namespace aphid {

PUBLISH(Param_aphid)
// Do not consider temperature yet
Param_aphid::Param_aphid(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(longevityImmigrant);
    Input(longevityAdult);
    Input(longevityNapt);
    Input(longevityNala);
}

void Param_aphid::reset() {

}

void Param_aphid::update() {

}
}
