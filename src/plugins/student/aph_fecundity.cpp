#include <base/publish.h>
#include "aph_fecundity.h"

using namespace base;

namespace student {

PUBLISH(Aph_fecundity)

Aph_fecundity::Aph_fecundity(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (proportion_alate);
    Input (density_apterous);
    Input (reproduction_rate);
    Output(nb_alates_nymph);
    Output(nb_apterous_nymph);
}

void Aph_fecundity::reset() {
    nb_alates_nymph=0;
}

void Aph_fecundity::update() {
    nb_alates_nymph= proportion_alate * density_apterous * reproduction_rate;
    nb_apterous_nymph= (1-proportion_alate)* density_apterous * reproduction_rate;
}


}
