#ifndef INFECTION_RATE3_H
#define INFECTION_RATE3_H
#include <base/box.h>

namespace aphid {

class Infection_rate3 : public base::Box
{
public:
    Infection_rate3(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double nb_spo_cadavers;
    double transmission_efficiency;
    double nb_susceptible;
    double nb_tillers;
    // Outputs
    double rho;
    double newly_infected;
    // Data
    double density_cadavers;
};

}

#endif
