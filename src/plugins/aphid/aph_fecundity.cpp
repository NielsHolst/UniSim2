#include <base/publish.h>
#include "aph_fecundity.h"

using namespace base;

namespace aphid {

PUBLISH(Aph_fecundity)

Aph_fecundity::Aph_fecundity(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (proportion_alate);
    Input (number_immigrants);
    Input (number_apterous);
    Input (reproduction_rate);
    Output(nb_alates_nymph);
    Output(nb_apterous_nymph);
}

void Aph_fecundity::reset() {
    nb_alates_nymph=
    nb_apterous_nymph=0;
}

void Aph_fecundity::update() {
    nb_adult_total = number_immigrants+number_apterous;

    nb_alates_nymph= proportion_alate * nb_adult_total * reproduction_rate;
    nb_apterous_nymph= (1-proportion_alate)* nb_adult_total * reproduction_rate;
}


}
