#ifndef DEMAND_CARBON_RESPIRATION_H
#define DEMAND_CARBON_RESPIRATION_H
#include <base/box.h>

namespace saccharina {

class DemandCarbonRespiration : public base::Box
{
public:
    DemandCarbonRespiration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double resp20, Q10, dryWeight, T, timeStep;
    // Outputs
    double value;
};

}

#endif
