#include <base/exception.h>
#include <base/publish.h>
#include "supply_budget.h"
using namespace base;

namespace boxes {

PUBLISH(SupplyBudget)

SupplyBudget::SupplyBudget(QString name, QObject *parent)
    : Box(name, parent) {
    help("supply energy budget");
    Input(resourceAcquired).help("Resource acquired");
    Input(respirationDemand).help("Demand for respiration");
    Input(egested).help("Egested or unused fraction of resource (prey) [0;1]");
    Input(conversionCost).help("Cost of converting resource into consumer [0;1]");
    Output(supply).help("Supply to meet the max. assimilation");
    Output(egestedAmount).help("Egested or unused amount resource");
    Output(conversionCostAmount).help("Cost of converting resource into consumer");
}

void SupplyBudget::update() {
        egestedAmount = egested*resourceAcquired;
        conversionCostAmount = conversionCost*(resourceAcquired - egestedAmount - respirationDemand);
        supply =  resourceAcquired - egestedAmount - conversionCostAmount - respirationDemand;
}

} //namespace

