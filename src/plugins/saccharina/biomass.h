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
    double structuralMass, structuralMassGrowth, kA, Cstruct, Nstruct, kdw, kN, kC, Nmin, Cmin, C, N, plantDensity, ropeDensity;
    // Outputs
    double Dryweight, Wetweight, carbonTotal, nitrogenTotal,
    nitrogenPct, carbonPct, DryweightprAlgae, DryweightprHour, DryweightYieldprmeter, WetweightYieldprmeter, CNratio;
};

}

#endif
