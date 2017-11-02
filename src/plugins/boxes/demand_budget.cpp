#include <base/exception.h>
#include <base/publish.h>
#include "demand_budget.h"
using namespace base;

namespace boxes {

PUBLISH(DemandBudget)

DemandBudget::DemandBudget(QString name, QObject *parent)
    : Box(name, parent) {
    help("demand energy budget");
    Input(netDemand).help("Net demand (growth, reproduction)");
    Input(respirationDemand).help("Demand for respiration");
    Input(egested).help("Egested or unused fraction of resource (prey) [0;1]");
    Input(conversionCost).help("Cost of converting resource into consumer [0;1]");
    Output(demand).help("Total demand of the whole energy budget");
}

void DemandBudget::update() {
    demand = (netDemand/(1. - conversionCost) + respirationDemand)/(1. - egested);
}

} //namespace

