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
        proportionExuded, demandErosion;
    // Outputs
    double
        MC0, MC1, MC2, MC3,
        MN0, MN1,
        supplyCarbonStructure, supplyCarbonExudation,
        structuralMassGrowth, structuralMassLoss,
        reservesProportionC, reservesProportionN,
        sdCarbon, sdNitrogen;

};

}

#endif
