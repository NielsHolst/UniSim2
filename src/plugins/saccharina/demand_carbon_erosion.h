#ifndef DEMAND_CARBON_EROSION_H
#define DEMAND_CARBON_EROSION_H
#include <base/box.h>

namespace saccharina {

class DemandCarbonErosion : public base::Box
{
public:
    DemandCarbonErosion(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double A, e50, eSlope, eMax, timeStepSecs,
        proportionC, kA;
    // Outputs
    double value;
};

}

#endif
