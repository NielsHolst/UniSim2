#include <base/publish.h>
#include "Infection_rate2.h"

using namespace base;

namespace aphid {

PUBLISH(Infection_rate2)

Infection_rate2::Infection_rate2(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (nb_spo_cadavers);
    Input(nb_new_spo_cadavers);
    Input (transmission_efficiency);
    Input(nb_susceptible);
    Output(rho);
    Output(newly_infected);
    Output(R_daugther_to_parent);
}

void Infection_rate2::reset() {
    rho=
    R_daugther_to_parent=
    newly_infected=
    density_cadavers=0;
}

void Infection_rate2::update() {
    //density_cadavers=nb_spo_cadavers/(nb_spo_cadavers+nb_susceptible);
    if(nb_susceptible>0) {density_cadavers=nb_spo_cadavers/nb_susceptible;}
    rho=1-exp(-transmission_efficiency*density_cadavers);
    newly_infected=rho*nb_susceptible;

    //Ratio daugther to parent to determine when there is an epizootic (cf: epizootiology of insect diseases p7)
    if(nb_new_spo_cadavers>=0.02) // because of super high value and infinity problem: max =50 nows
    {R_daugther_to_parent=newly_infected/nb_new_spo_cadavers;}

    if(nb_new_spo_cadavers<0.02)
    {R_daugther_to_parent=newly_infected/0.02;}
}
}
