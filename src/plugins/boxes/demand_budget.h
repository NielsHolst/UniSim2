#ifndef DEMAND_BUDGET
#define DEMAND_BUDGET
#include <base/box.h>

namespace boxes {

class DemandBudget : public base::Box
{
public: 
    DemandBudget(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double netDemand, respirationDemand, egested, conversionCost;
    // Outputs
    double demand;
};

} //namespace
#endif
