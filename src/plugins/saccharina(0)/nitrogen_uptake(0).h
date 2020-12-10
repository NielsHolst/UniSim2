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
    double nitrogenDemand, fCurrent,
        N, KN;
    // Outputs
    double supply, sdRatio;
};

}

#endif
