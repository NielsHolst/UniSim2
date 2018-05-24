#ifndef INFECTION_RATE_H
#define INFECTION_RATE_H
#include <base/box.h>

namespace aphid {

class Infection_rate : public base::Box
{
public:
    Infection_rate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double nb_spo_cadavers;
    double transmission_efficiency;
    double nb_susceptible;
    // Outputs
    double rho;
    double newly_infected;
};

}

#endif
