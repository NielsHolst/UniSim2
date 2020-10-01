#ifndef CARBON_RESERVE_H
#define CARBON_RESERVE_H
#include <base/box.h>

namespace saccharina {

class CarbonReserve : public base::Box
{
public:
    CarbonReserve(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double initialValue, kA, P, E, R, Cstruct, u, timeStepDays,
        A, Cmin;
    // Outputs
    double value, gain, loss, areaLost;
};

}

#endif
