#ifndef FUNGALMORTALITY_H
#define FUNGALMORTALITY_H
#include <base/box.h>

namespace aphid {

class FungalMortality : public base::Box
{
public:
    FungalMortality(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
//    double intrinsic_mortality;
    double fungal_mortality;
    double first_infection;
    double nb_N_apterous;
    double nb_A_apterous;
    double nb_N_alates;
    // Outputs
//    double mortality_N_apterous;
//    double mortality_N_alates;
//    double mortality_A_apterous;
    double new_infected_A_apterous;
    double new_infected_N_apterous;
    double new_infected_N_alates;
    // Data
    double flag_first_infection;

};

}

#endif
