#ifndef SUPPLY_BUDGET
#define SUPPLY_BUDGET
#include <base/box.h>

namespace boxes {

class SupplyBudget : public base::Box
{
public: 
    SupplyBudget(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double resourceAcquired, respirationDemand, egested, conversionCost;
    // Outputs
    double supply, egestedAmount, conversionCostAmount;
};

} //namespace
#endif
