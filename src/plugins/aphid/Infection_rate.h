#ifndef INFECTION_RATE_H
#define INFECTION_RATE_H
#include <base/box.h>

namespace aphid {

class InfectionRate: public base::Box
{
public:
    InfectionRate(QString name, QObject *parent);
    void update();
private:
    // Inputs
    bool isSporulating;
    double cadavers, transmissionEfficiency;
    // Outputs
    double value;

};

}

#endif
