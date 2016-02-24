#ifndef DISTRIBUTED_DELAY_H
#define DISTRIBUTED_DELAY_H

#include "distributed_delay_base.h"

namespace boxes {

class DistributedDelay : public DistributedDelayBase
{
public:
    struct Parameters {
        double L;
        int k, minIter;
    };
    struct State {
        double outflowRate, growthRate;
        State() : outflowRate(0), growthRate(0) {}
    };

    DistributedDelay(const Parameters &p, QObject *parent);
    DistributedDelay(const DistributedDelay &dd);
    void update(double inflow, double dt, double fgr);
    State state() const;


private:
    // data
    Parameters p;
    State s;
};

}
#endif
