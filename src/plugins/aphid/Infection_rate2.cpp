#include <base/publish.h>
#include "Infection_rate2.h"

using namespace base;

namespace aphid {

PUBLISH(Infection_rate2)

Infection_rate2::Infection_rate2(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (nb_spo_cadavers);
    Input (transmission_efficiency);
    Input(nb_susceptible);
    Output(rho);
    Output(newly_infected);
}

void Infection_rate2::reset() {
    rho=0;
}

void Infection_rate2::update() {
    //density_cadavers=nb_spo_cadavers/(nb_spo_cadavers+nb_susceptible);
    density_cadavers=nb_spo_cadavers/nb_susceptible;
    rho=1-exp(-transmission_efficiency*density_cadavers);
    newly_infected=rho*nb_susceptible;
}
}
