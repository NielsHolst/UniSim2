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
    Input (nb_adult_apterous);
    Input (nb_adult_alates);
    Input (nb_plant);
    Output(density);
}

void Density_per_tiller::reset() {
    density = 0;
}

void Density_per_tiller::update() {
    density = (nb_nymph_apterous+nb_nymph_alates+nb_adult_apterous +nb_adult_alates)/(nb_plant);
    // the adults alates participate to the density measure just before to leave the field.
}


}
