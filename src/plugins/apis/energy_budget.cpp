#include <base/any_year.h>
#include <base/publish.h>
#include "energy_budget.h"

using namespace base;

namespace apis {

PUBLISH(EnergyBudget)

EnergyBudget::EnergyBudget(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(supply).imports("honeySupply[potentialSupply]");
    Input(demandRespiration).imports("demandRespiration[demandRespiration]");
    Input(demandGrowth).imports("demandRespiration[demandGrowth]");
    Input(storeHolding).imports("honeyStore[holding]");
    Input(storeCapacity).imports("honeyStore[capacity]");
    Output(realisedSupply);
    Output(supplyRespiration);
    Output(supplyGrowth);
    Output(supplyStore);
    Output(supplyDemandGrowth);
}

void EnergyBudget::reset() {
}

void EnergyBudget::update() {
    double balance = storeHolding + supply - demandRespiration - demandGrowth;
    // Cut potential supply to store capacity
    realisedSupply = (balance > storeCapacity) ? supply - balance : supply;
    // Compute the ressource avaible for allocation
    double available = storeHolding + realisedSupply;
    // Allocate to respiration
    if (available >= demandRespiration) {
        available -= supplyRespiration = demandRespiration;
        // Allocate to growth
        if (available >= demandGrowth)
            available -= supplyGrowth = demandGrowth;
        else {
            supplyGrowth = available;
            available = 0;
        }
    }
    else {
        supplyRespiration = available;
        available = supplyGrowth = 0;
    }
    // Allocate to store
    supplyStore = realisedSupply - supplyRespiration - supplyGrowth;
    // Supply:demand ratios
    supplyDemandGrowth = (demandGrowth > 0.) ? supplyGrowth/demandGrowth : 0.;
}

}
