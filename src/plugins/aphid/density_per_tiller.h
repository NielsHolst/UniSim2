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
    double nb_plant;
    // Outputs
    double density;
};

}

#endif
