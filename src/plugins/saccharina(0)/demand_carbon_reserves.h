#ifndef DEMAND_CARBON_RESERVES_H
#define DEMAND_CARBON_RESERVES_H
#include <base/box.h>

namespace saccharina {

class DemandCarbonReserves : public base::Box
{
public:
    DemandCarbonReserves(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double reservesProportionC, maxReservesProportionC,
        structProportionC, structuralMass, demandStructureC;
    // Outputs
    double value;
};

}

#endif
