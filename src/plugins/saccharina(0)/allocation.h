#ifndef ALLOCATION_H
#define ALLOCATION_H
#include <base/box.h>

namespace saccharina {

class Allocation : public base::Box
{
public:
    Allocation(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        structuralProportionC, structuralProportionN, structuralMass,
        curReservesProportionC, curReservesProportionN,
        reservesMaxProportionC, reservesMaxProportionN,
        supplyCarbonTotal, supplyNitrogenTotal,
        demandCarbonRespiration,
        demandCarbonStructure, demandNitrogenStructure,
        proportionExuded;
    // Outputs
    double
        MC0, MC1, MC2, MC3,
        MN0, MN1,
        supplyCarbonStructure, supplyCarbonExudation,
        structuralMassGrowth, structuralMassLoss, structuralMassLossRate,
        reservesProportionC, reservesProportionN,
        sdCarbon, sdNitrogen;

};

}

#endif
