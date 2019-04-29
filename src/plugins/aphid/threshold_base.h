#ifndef THRESHOLD_BASE_H
#define THRESHOLD_BASE_H
#include <base/box.h>

namespace aphid {

class ThresholdBase : public base::Box
{
public:
    ThresholdBase(QString name, QObject *parent);
    void reset();
protected:
    // Inputs
    double cropGrowthStage, aphids;
    // Outputs
    bool exceeded;
    double exceededAtGS;

};

}

#endif
