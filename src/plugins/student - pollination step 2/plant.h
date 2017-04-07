// Description: plant class--floral rewards a separate class?
#ifndef PLANT_H
#define PLANT_H
#include <base/box.h>

namespace student {

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
    double preference; // preference of pollinator for eating floral rewards
    double B0; // half-saturation density
    double h; // Hill coefficient
    double cr;

    // for Allometry
    double m; // body mass of plant
    bool is_vertebrate; // 1 if vertebrate
    //    calculated in Allometry
    double x; // metabolic rate of pollinator
    double y; // maximum consumption rate of pollinator eating floral rewards
    double r; // intrinsic growth rate of plants

    //    for dRdt, Rate of Floral Rewards Change
    double B; // pollinator biomass
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
