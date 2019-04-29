#include <base/publish.h>
#include "infection_rate.h"

using namespace base;

namespace aphid {

PUBLISH(InfectionRate)

InfectionRate::InfectionRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the proportion of hosts being infected per day");
    Input(isSporulating).help("Are cadavers sporulating?").unit("boolean");
    Input(cadavers).help("Sporulating cadaverss").unit("per tiller");
    Input(transmissionEfficiency).help("Transmission efficiency ").unit("per cadaver per day");
    Output(value);
}

void InfectionRate::update() {
    value = isSporulating ? 1. - exp(-transmissionEfficiency*cadavers) : 0.;
}

}
