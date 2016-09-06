#ifndef STAGE_H
#define STAGE_H


#include "distributed_delay.h"
#include "stage_base.h"

namespace boxes {

class Stage : public StageBase
{
public:
    Stage(QString name, QObject *parent);
    void reset();
    void update();

    // special methods
    DistributedDelayBase* createDistributedDelay();
private:
    // inputs
    double timeStep, inflow, initial, phaseOutflowProportion;

    // outputs
    double latestInflow, outflow;

    // data
    bool firstUpdate;
    double inflowPending;
    DistributedDelay *dd;
};

}
#endif
