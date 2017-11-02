#ifndef STAGE_AND_PHASE_H
#define STAGE_AND_PHASE_H

#include "distributed_delay_2d.h"
#include "stage_base.h"

namespace boxes {

class DistributedDelay2D;

class StageAndPhase : public StageBase
{
public:
    StageAndPhase(QString name, QObject *parent);
    // standard methods
    void reset();
    void update();

    // special methods
    void createDistributedDelay();
    double growthDemand();

private:
    // inputs
    int phaseK;
    double phaseDuration, timeStep, phaseTimeStep;
    QVector<double> inflow;

    // outputs
    QVector<double> outflowCohorts, latestInflow, latestPhaseInflow;
    double outflowScalar;

    // data
    QVector<double> inflowPending, phaseInflowPending;
    DistributedDelay2D *_dd;
};

}
#endif
