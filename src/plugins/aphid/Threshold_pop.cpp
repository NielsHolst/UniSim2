#include <base/publish.h>
#include "Threshold_pop.h"

using namespace base;

namespace aphid {

PUBLISH(Threshold_pop)

Threshold_pop::Threshold_pop(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (fungal_mortality);
    Input(natural_survival);
    Input (transmission_capa);
    Output(Ht);
}

void Threshold_pop::reset() {
    Ht=0;
}

void Threshold_pop::update() {
    Ht=round((fungal_mortality + (1-natural_survival))/transmission_capa);
}
}
