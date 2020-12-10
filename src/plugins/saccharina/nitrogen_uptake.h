#ifndef NITROGEN_UPTAKE_H
#define NITROGEN_UPTAKE_H
#include <base/box.h>

namespace saccharina {

class NitrogenUptake : public base::Box
{
public:
    NitrogenUptake(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double beta, demand, area, lai, fCurrent, N, timeStepSecs, dryWeight;
    // Outputs
    double supply, sdRatio, J, V, B;
};

}

#endif
