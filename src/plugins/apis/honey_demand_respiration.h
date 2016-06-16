#ifndef HONEY_DEMAND_RESPIRATION_H
#define HONEY_DEMAND_RESPIRATION_H
#include <base/box.h>

namespace apis {

class HoneyDemandRespiration : public base::Box
{
public:
    HoneyDemandRespiration(QString name, QObject *parent);
    void update();
private:
    // input
    QVector<double> cohortNumbers;
    double timeStepDays, respirationRate;
    // output
    double value;
};

}

#endif
