#ifndef YIELD_H
#define YIELD_H
#include <base/box.h>

namespace aphid {

class Yield : public base::Box
{
public:
    Yield(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double cropGrowthStage, aphidIndex;
    // Outputs
    double yield, loss, lossRate;
};

}

#endif
