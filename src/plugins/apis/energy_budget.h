#ifndef ENERGY_BUDGET_H
#define ENERGY_BUDGET_H
#include <base/box.h>

namespace apis {

class EnergyBudget : public base::Box
{
public:
    EnergyBudget(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    double supplyPotential, demandRespiration, demandGrowth, costGrowth,
        storeHolding, storeCapacity;
    // output
    double supplyAvailable, supplyRespiration, supplyGrowth, changeInStore,
        supplyDemandRespiration, supplyDemandGrowth;
};

}

#endif
