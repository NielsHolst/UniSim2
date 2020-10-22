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
    double r1, TAR, TR1, T, area, timeStep;
    // Outputs
    double value;
};

}

#endif
