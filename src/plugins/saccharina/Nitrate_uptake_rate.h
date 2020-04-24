#ifndef NITRATE_UPTAKE_RATE_H
#define NITRATE_UPTAKE_RATE_H
#include <base/box.h>

namespace saccharina {

class Nitrateuptakerate : public base::Box
{
public:
    Nitrateuptakerate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Jmax, X, KX, N, Nmin, Nmax, U, U065;
    // Outputs
    double Nuptakerate;
};

}

#endif
