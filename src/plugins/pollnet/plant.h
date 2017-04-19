// Description: plant class--floral rewards a separate class?
#ifndef PLANT_H
#define PLANT_H
#include <base/box.h>

namespace pollnet {

class Plant : public base::Box
{
public:
    Plant(QString name, QObject *parent);
    void reset();
    void update(); // compute_new_R

private:
    // Inputs
    double initial_R;
    double initial_V;

    //    for F, Functional Response
    double CR; // consumption rate

    // for Allometry
    double m; // body mass of plant

    //    calculated in Allometry
    double x; // metabolic rate of plant
    double r; // intrinsic growth rate of plant

    //    for dRdt, Rate of Floral Rewards Change
    double s; // self-limitation rate of floral rewards
    double beta; // production rate of floral rewards

    //    for dVdt, Rate of Vegetative Biomass Change
    double K; // plant community-wide carrying capacity
    double kappa; // cost of producing floral rewards

    double Vsum;

    // Outputs
    double R; // floral rewards biomass
    double V; // plant vegetative biomass


    // Methods
    double compute_floral_growth();
    double compute_pollinator_consumption();
    double compute_benefit_accrual();
    double compute_K_effect();
    double compute_dRdt();
    double compute_dVdt();

};

}

#endif
