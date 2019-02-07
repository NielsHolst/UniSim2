#include <base/publish.h>
#include "Infection_rate3.h"

using namespace base;

namespace aphid {

PUBLISH(Infection_rate3)

Infection_rate3::Infection_rate3(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (nb_spo_cadavers);
    Input (transmission_efficiency);
    Input(nb_susceptible);
    Input(nb_tillers);
    Output(rho);
    Output(newly_infected);
}

void Infection_rate3::reset() {
    rho=0;
}

void Infection_rate3::update() {
    density_cadavers=nb_spo_cadavers/nb_tillers;
    rho=1-exp(-transmission_efficiency*density_cadavers);
    newly_infected=rho*nb_susceptible;
}
}
