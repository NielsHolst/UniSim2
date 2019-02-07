#ifndef DENSITY_PER_TILLER_H
#define DENSITY_PER_TILLER_H
#include <base/box.h>

namespace aphid {

class Density_per_tiller : public base::Box
{
public:
    Density_per_tiller(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double nb_nymph_apterous;
    double nb_nymph_alates;
    double nb_pre_adults;
    double nb_adult_apterous;
    double nb_adult_alates;
    double nb_Aapt_infec;
    double nb_Napt_infec;
    double nb_Nala_infec;
    double nb_immi_infec;
    double nb_plant;
    double nb_W_produced; // just for the output of the model
    // Outputs
    double density; // total density all biological stages taken into account
    double d_N_apt;
    double d_N_ala;
    double d_A_apt;
    double d_A_ala; //immigrants only the winged individuals produced leave the spot
    double d_N_apt_infect;
    double d_N_ala_infect;
    double d_A_apt_infect;
    double d_immi_infect;
    double d_W_produced; // just for the output of the model
    // Methods
    void setToZero();
};

}

#endif
