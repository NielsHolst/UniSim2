#include <base/publish.h>
#include "density_per_tiller.h"

using namespace base;

namespace aphid {

PUBLISH(Density_per_tiller)

Density_per_tiller::Density_per_tiller(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (nb_nymph_apterous);
    Input (nb_nymph_alates);
    Input(nb_pre_adults);
    Input (nb_adult_apterous);
    Input (nb_adult_alates);
    Input(nb_Napt_infec);
    Input(nb_Aapt_infec);
    Input (nb_plant);
    Output(density);
}

void Density_per_tiller::reset() {
    density = 0;
}

void Density_per_tiller::update() {
    density = (nb_nymph_apterous+nb_nymph_alates+nb_pre_adults+nb_adult_apterous +nb_adult_alates+nb_Aapt_infec+nb_Napt_infec)/nb_plant;
    // the adults alates participate to the density measure when immigrants.
}


}
