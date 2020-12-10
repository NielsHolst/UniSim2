#ifndef BIOMASS_H
#define BIOMASS_H
#include <base/box.h>

namespace saccharina {

class Biomass : public base::Box
{
public:
    Biomass(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double structuralMass, structuralMassGrowth, kA, Cstruct, Nstruct, kdw, kN, kC, C, N, plantDensity, ropeDensity;
    // Outputs
    double dryWeight, wetWeight, carbonWeight, nitrogenWeight,
    nitrogenPct, carbonPct, dryWeightGrowth, dryWeightYield, wetWeightYield, CNratio;
};

}

#endif
