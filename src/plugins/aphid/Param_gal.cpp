#include <base/publish.h>
#include "Param_gal.h"

using namespace base;

namespace aphid {

PUBLISH(Param_gal)
// Do not consider temperature yet
Param_gal::Param_gal(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(k);
    Input(kReproHealthy);
}

void Param_gal::reset() {

}

void Param_gal::update() {

}
}
