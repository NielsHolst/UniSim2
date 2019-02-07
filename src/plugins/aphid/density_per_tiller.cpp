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
    Input (nb_immi_infec);
    Input(nb_Napt_infec);
    Input(nb_Aapt_infec);
    Input (nb_Nala_infec);
    Input (nb_plant);
    Input (nb_W_produced);
    Output(density); // total density, all biological stages taken into account
    Output(d_N_apt);
    Output(d_N_ala);
    Output(d_A_apt);
    Output(d_A_ala); //immigrants only the winged individuals produced leave the spot
    Output(d_N_apt_infect);
    Output(d_N_ala_infect);
    Output(d_A_apt_infect);
    Output(d_immi_infect);
    Output(d_W_produced);
}

void Density_per_tiller::reset() {
    setToZero();
}

void Density_per_tiller::update() {
    if (nb_plant == 0.) {
        setToZero();
    }
    else {
        density = (nb_nymph_apterous+nb_nymph_alates+nb_pre_adults+nb_adult_apterous +nb_adult_alates+ nb_immi_infec+ nb_Aapt_infec+ nb_Napt_infec + nb_Nala_infec)/nb_plant;
        // the adults alates participate to the density measure when immigrants.
        d_N_apt=(nb_nymph_apterous)/nb_plant;
        d_N_ala=(nb_nymph_alates)/nb_plant;
        d_A_apt=(nb_adult_apterous)/nb_plant;
        d_A_ala=(nb_adult_alates)/nb_plant;
        d_N_apt_infect=(nb_Napt_infec)/nb_plant;
        d_N_ala_infect=(nb_Nala_infec)/nb_plant;
        d_A_apt_infect=(nb_Aapt_infec)/nb_plant;
        d_immi_infect=(nb_immi_infec)/nb_plant;

        d_W_produced=(nb_W_produced)/nb_plant; // just for output model
    }
}

void Density_per_tiller::setToZero() {
    density =
    d_N_apt =
    d_N_ala =
    d_A_apt =
    d_A_ala =
    d_immi_infect=
    d_A_apt_infect=
    d_N_apt_infect =
    d_N_ala_infect =
    d_W_produced = 0;
}

}
