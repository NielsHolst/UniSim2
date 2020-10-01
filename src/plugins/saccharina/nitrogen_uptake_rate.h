#ifndef NITROGEN_UPTAKE_RATE_H
#define NITROGEN_UPTAKE_RATE_H
#include <base/box.h>

namespace saccharina {

class NitrogenUptakeRate : public base::Box
{
public:
    NitrogenUptakeRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Jmax, X, KX, N, Nmin, Nmax, U, U65, a, b;
    // Outputs
    double value;
};

}

#endif
