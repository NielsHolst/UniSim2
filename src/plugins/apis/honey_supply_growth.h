#ifndef HONEY_SUPPLY_GROWTH_H
#define HONEY_SUPPLY_GROWTH_H
#include <base/box.h>

namespace apis {

class HoneySupplyGrowth : public base::Box
{
public:
    HoneySupplyGrowth(QString name, QObject *parent);
    void update();
private:
    // input
    QVector<double> cohortDemands;
    double cost;
    // output
    QVector<double> cohortSupplies ;
};

}

#endif
