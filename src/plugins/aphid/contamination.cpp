#include <base/publish.h>
#include "contamination.h"

using namespace base;

namespace aphid {

PUBLISH(Contamination)

Contamination::Contamination(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(virulence_strain);
    Input(transmission_efficiency);
    Output(proba_infection);
}

void Contamination::reset() {
    proba_infection=0;
}

void Contamination::update() {
    proba_infection=virulence_strain*transmission_efficiency;
}


}
