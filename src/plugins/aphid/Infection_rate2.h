#ifndef INFECTION_RATE2_H
#define INFECTION_RATE2_H
#include <base/box.h>

namespace aphid {

class Infection_rate2 : public base::Box
{
public:
    Infection_rate2(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double nb_spo_cadavers;
    double nb_new_spo_cadavers;
    double transmission_efficiency;
    double nb_susceptible;
    // Outputs
    double rho;
    double newly_infected;
    double R_daugther_to_parent;
    // Data
    double density_cadavers;
};

}

#endif
