#ifndef STAGE_BASE_H
#define STAGE_BASE_H

#include <QVector>
#include <base/box.h>
#include "distributed_delay_base.h"

namespace boxes {

class StageBase : public base::Box
{
public:
    StageBase(QString name, QObject *parent);
    ~StageBase();
    void reset();

    const double *data();
    virtual void createDistributedDelay() = 0;

protected:
    // inputs
    int k;
    double initial, duration, growthFactor, sdRatio, instantLossRate, zeroLimit;
    QVector<double> phaseInflow;

    // outputs
    double content, inflowTotal, outflowTotal, phaseInflowTotal, phaseOutflowTotal, growth;
    QVector<double> phaseOutflow;

    // methods
    void applyInstantMortality(double mortality);

    // data
    DistributedDelayBase *_ddBase;
};

}
#endif
