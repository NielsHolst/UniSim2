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
    double supply, demandRespiration, demandGrowth,
        storeHolding, storeCapacity;
    // output
    double realisedSupply, supplyRespiration, supplyGrowth, supplyStore,
        supplyDemandGrowth;
};

}

#endif
