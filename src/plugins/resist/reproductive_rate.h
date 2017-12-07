#ifndef REPRODUCTIVE_RATE_H
#define REPRODUCTIVE_RATE_H
#include <base/box.h>

namespace resist {

class ReproductiveRate : public base::Box
{
public:
    ReproductiveRate(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double R, fitnessPenalty, Ntotal, K;
    // Outputs
    double ss, sr, rr;
    QVector<double> values;
};

}

#endif
