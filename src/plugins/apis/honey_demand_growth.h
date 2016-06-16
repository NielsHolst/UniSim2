#ifndef HONEY_DEMAND_GROWTH_H
#define HONEY_DEMAND_GROWTH_H
#include <base/box.h>

namespace apis {

class HoneyDemandGrowth : public base::Box
{
public:
    HoneyDemandGrowth(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    QVector<double> cohortNumbers, cohortDemands;
    double cost;
    // output
    double value;
};

}

#endif
