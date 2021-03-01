#ifndef RAIN_MORTALITY_H
#define RAIN_MORTALITY_H
#include <base/box.h>

namespace student {

class RainMortality : public base::Box
{
public:
    RainMortality(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double rain, beta, timeStep;
    // Outputs
    double value;
};

}

#endif
