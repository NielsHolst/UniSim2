// Description: pollinator class
#ifndef POLLINATOR_H
#define POLLINATOR_H
#include <base/box.h>

namespace student {

class Pollinator : public base::Box
{
public:
    Pollinator(QString name, QObject *parent);
    void reset();
    void update(); // compute_new_B

private:

    // Inputs
    double initial_B;

    //    for F, Functional Response
    double R; // floral rewards biomass
    double preference; // preference of pollinator for eating floral rewards
    double B0; // half-saturation density
    double h; // Hill coefficient

    // for Allometry
    double m; // body mass of pollinator
    bool is_vertebrate; // 1 if vertebrate

    //    for dBdt, Rate of Biomass Change
    double x; // metabolic rate of pollinator
    double y; // maximum consumption rate of pollinator eating floral rewards
    double cr;

    // Outputs
    double B; // pollinator biomass

    // Methods
    double compute_dBdt();

};

}

#endif
