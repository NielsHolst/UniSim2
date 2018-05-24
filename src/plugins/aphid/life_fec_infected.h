#ifndef LIFE_FEC_INFECTED_H
#define LIFE_FEC_INFECTED_H
#include <base/box.h>

namespace aphid {

class life_fec_infected : public base::Box
{
public:
    life_fec_infected(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double life_fec_apt;
    double life_fec_ala;
    double reduction; // percentage of fecundity loss
    // Outputs
    double life_fec_aptInfected;
    double life_fec_alaInfected;
};

}

#endif
