#ifndef SPRAY_SURVIVAL_H
#define SPRAY_SURVIVAL_H
#include <base/box.h>

namespace resist {

class SpraySurvival: public base::Box
{
public:
    SpraySurvival(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double coverage, maxMortality, slope, resistanceGain;
    // Output
    double ss, sr, rr;
        QVector<double> values;
};

}

#endif
