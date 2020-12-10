#ifndef DEMAND_NITROGEN_RESERVES_H
#define DEMAND_NITROGEN_RESERVES_H
#include <base/box.h>

namespace saccharina {

class DemandNitrogenReserves : public base::Box
{
public:
    DemandNitrogenReserves(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double reservesProportionN, maxReservesProportionN,
        structuralMass, structProportionC, demandStructureC;
    // Outputs
    double value;
};

}

#endif
