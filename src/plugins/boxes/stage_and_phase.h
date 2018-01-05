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
    void myReset();
    void update();

    // special methods
    void createDistributedDelay();

private:
    // inputs
    int phaseK;
    double phaseDuration, phaseTimeStep;
    QVector<double> inflow;

    // outputs
    QVector<double> outflow;

    // data
    bool _firstUpdate;
    DistributedDelay2D *_dd;
};

}
#endif