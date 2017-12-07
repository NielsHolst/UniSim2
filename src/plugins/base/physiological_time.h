#ifndef BASE_PHYSIOLOGICAL_TIME
#define BASE_PHYSIOLOGICAL_TIME
#include "box.h"

namespace base {

class PhysiologicalTime : public Box
{
public:
    PhysiologicalTime(QString name, QObject *parent);
    void update() final;
    virtual void updateStep() = 0;
protected:
    // Inputs
    double T, timeStepDays;
    bool resetTotal, isTicking;
    // Outputs
    double step, total;
};

}
#endif
