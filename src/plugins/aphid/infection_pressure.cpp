#include <base/publish.h>
#include "Infection_pressure.h"

using namespace base;

namespace aphid {

PUBLISH(Infection_pressure)

Infection_pressure::Infection_pressure(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (cadavers_N_apterous);
    Input (cadavers_A_apterous);
    Input (cadavers_N_alate);
    Input (density_aphid);// has to be the density of healthy aphids.
    Input (density_aphid_threshold);
    Input (sporulation_capacity);
    Output (nb_sporulating_cadavers);
    Output(infection_capacity);
}

void Infection_pressure::reset() {
    infection_capacity=
    cadavers_N_apterous=
    cadavers_A_apterous=
    cadavers_N_alate=
    cadavers_ready_to_sporulate_D1=
    cadavers_ready_to_sporulate_D2=
    cadavers_ready_to_sporulate_D3=
    cadavers_ready_to_sporulate_D4=
    cadavers_ready_to_sporulate_D5=
    cadavers_ready_to_sporulate_D6=
    cadavers_ready_to_sporulate_D7=
    cadavers_ready_to_sporulate=0;
}

void Infection_pressure::update() {
//1. we adjust the age of the cadavers which have not sporulated yet:
    cadavers_ready_to_sporulate_D7=cadavers_ready_to_sporulate_D6;
    cadavers_ready_to_sporulate_D6=cadavers_ready_to_sporulate_D5;
    cadavers_ready_to_sporulate_D5=cadavers_ready_to_sporulate_D4;
    cadavers_ready_to_sporulate_D4=cadavers_ready_to_sporulate_D3;
    cadavers_ready_to_sporulate_D3=cadavers_ready_to_sporulate_D2;
    cadavers_ready_to_sporulate_D2=cadavers_ready_to_sporulate_D1;
    cadavers_ready_to_sporulate_D1=cadavers_N_apterous+cadavers_A_apterous+cadavers_N_alate;
    cadavers_ready_to_sporulate=cadavers_ready_to_sporulate_D1+cadavers_ready_to_sporulate_D2+cadavers_ready_to_sporulate_D3+cadavers_ready_to_sporulate_D4+cadavers_ready_to_sporulate_D5+cadavers_ready_to_sporulate_D6+cadavers_ready_to_sporulate_D7;
//2. we calcul the number of cadavers sporulating today:
    nb_sporulating_cadavers=cadavers_ready_to_sporulate*sporulation_capacity;
//becarefull for the moment a cadaver can sporulate for a week. Iti si too long.
//3. we adjust the number of cadavers which are ready to sporulate for tomorrow:
    cadavers_ready_to_sporulate_D7=cadavers_ready_to_sporulate_D7*(1-sporulation_capacity);
    cadavers_ready_to_sporulate_D6=cadavers_ready_to_sporulate_D6*(1-sporulation_capacity);
    cadavers_ready_to_sporulate_D5=cadavers_ready_to_sporulate_D5*(1-sporulation_capacity);
    cadavers_ready_to_sporulate_D4=cadavers_ready_to_sporulate_D4*(1-sporulation_capacity);
    cadavers_ready_to_sporulate_D3=cadavers_ready_to_sporulate_D3*(1-sporulation_capacity);
    cadavers_ready_to_sporulate_D2=cadavers_ready_to_sporulate_D2*(1-sporulation_capacity);
    cadavers_ready_to_sporulate_D1=cadavers_ready_to_sporulate_D1*(1-sporulation_capacity);

    if(density_aphid<density_aphid_threshold)
    infection_capacity=(nb_sporulating_cadavers*2)/density_aphid;

    else {infection_capacity=(nb_sporulating_cadavers*7)/density_aphid;}

    if(infection_capacity>1)
       infection_capacity=1;
}
}
