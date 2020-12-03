#ifndef DEMAND_CARBON_STRUCTURE_H
#define DEMAND_CARBON_STRUCTURE_H
#include <base/box.h>

namespace saccharina {

class DemandCarbonStructure : public base::Box
{
public:
    DemandCarbonStructure(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double maxGrowthRate, proportionC, structuralMass, fTemp, fSalinity, fArea, timeStep;
    // Outputs
    double value;
};

}

#endif
