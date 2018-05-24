#include <base/publish.h>
#include "fecundity_schmitz.h"

using namespace base;

namespace aphid {

PUBLISH(Fecundity_Schmitz)

Fecundity_Schmitz::Fecundity_Schmitz(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (potential_fec_alates);
    Input (potential_fec_apterous);
    Input(aphid_density);
    Input (Wheat_Pheno);
    Output(nb_alates_nymph);
    Output(nb_apterous_nymph);
    Output(perc_alates);
}

void Fecundity_Schmitz::reset() {
    perc_alates=
    nb_alates_nymph=
    nb_apterous_nymph=0;
}

void Fecundity_Schmitz::update() {
    nb_nymph_total=potential_fec_alates + potential_fec_apterous;
    //Carter et al. 1982 used in Plantegnest et al. 2001 and Duffy et al. 2017

    perc_alates= 2.603* aphid_density + 0.847*Wheat_Pheno -27.189;
    // !! aphid density here is per tiller according to Duffy et al 2017
    if(perc_alates>100){perc_alates=100;}
    if(perc_alates<0){perc_alates=0;}

    nb_apterous_nymph=((100-perc_alates)/100)* nb_nymph_total;
    nb_alates_nymph= (perc_alates/100)* nb_nymph_total;
}


}
