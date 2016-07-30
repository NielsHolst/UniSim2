#ifndef HONEY_DEMAND_RESPIRATION_H
#define HONEY_DEMAND_RESPIRATION_H
#include <base/box.h>

namespace apis {

class HoneyDemandRespiration : public base::Box
{
public:
    HoneyDemandRespiration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    double workers, respirationRate, timeStepSecs;
    // output
    double value, rate, total;
};

}

#endif
