#ifndef NITROGEN_RESERVE_H
#define NITROGEN_RESERVE_H
#include <base/box.h>

namespace saccharina {

class NitrogenReserve : public base::Box
{
public:
    NitrogenReserve(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double initialValue, kA, J, u, Nstruct, timeStepDays;
    // Outputs
    double value, gain, loss;
};

}

#endif
