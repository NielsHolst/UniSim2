#include <base/publish.h>
#include "Infection_rate.h"

using namespace base;

namespace aphid {

PUBLISH(Infection_rate)

Infection_rate::Infection_rate(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (nb_spo_cadavers);
    Input (transmission_efficiency);
    Input(nb_susceptible);
    Output(rho);
    Output(newly_infected);
}

void Infection_rate::reset() {
    rho=0;
}

void Infection_rate::update() {
    rho=1-exp(-transmission_efficiency*nb_spo_cadavers);
    newly_infected=rho*nb_susceptible;
}
}
