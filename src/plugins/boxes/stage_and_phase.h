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

private:
    // inputs
    int phaseK;
    double phaseDuration, timeStep, phaseTimeStep;
    QVector<double> inflow;

    // outputs
    QVector<double> stageOutflow, latestInflow, latestPhaseInflow;
    double stageOutflowSum, phaseOutflowSum;

    // data
    bool _firstUpdate;
    QVector<double> _inflowPending, _phaseInflowPending;
    DistributedDelay2D *_dd;
};

}
#endif
