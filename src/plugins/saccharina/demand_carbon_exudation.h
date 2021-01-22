#ifndef DEMAND_CARBON_EXUDATION_H
#define DEMAND_CARBON_EXUDATION_H
#include <base/box.h>

namespace saccharina {

class DemandCarbonExudation : public base::Box
{
public:
    DemandCarbonExudation(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double gamma, reservesProportion, structuralMass, demandStructure, timeStepSecs, exudation, wetWeight;
    // Outputs
    double value, proportion;
};

}

#endif
