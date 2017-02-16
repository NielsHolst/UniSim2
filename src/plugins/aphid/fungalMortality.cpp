#include <base/publish.h>
#include "fungalMortality.h"

using namespace base;

namespace aphid {

PUBLISH(FungalMortality)

FungalMortality::FungalMortality(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (fungal_mortality);// proportion of ongoing infection pressure
    Input (first_infection);// proportion of first infection pressure
    Input (nb_N_apterous); // density of N apterous
    Input (nb_A_apterous); // density of A apterous
    Input (nb_N_alates); // density of N alates
    Output(new_infected_A_apterous);
    Output (new_infected_N_apterous);
    Output (new_infected_N_alates);
}

void FungalMortality::reset() {
flag_first_infection=0;

new_infected_A_apterous=
new_infected_N_apterous=
new_infected_N_alates=0;
}

void FungalMortality::update() {
// Let's suppose for now that the different stages of aphids are evenly infected
// and that the intrinsic mortality is the ssame for all the stages as well.
    if (flag_first_infection<7) // I assume that a sporulating cadaver can produce infectible spores for 1 week (probably not true)
    {new_infected_A_apterous=first_infection*nb_A_apterous;
    new_infected_N_apterous=first_infection*nb_N_apterous;
    new_infected_N_alates=first_infection*nb_N_alates;
    flag_first_infection=flag_first_infection+1;}

    else
    {new_infected_A_apterous=fungal_mortality*nb_A_apterous;
     new_infected_N_apterous=fungal_mortality*nb_N_apterous;
     new_infected_N_alates=fungal_mortality*nb_N_alates;}
}

}
