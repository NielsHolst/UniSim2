#ifndef DENSITY_H
#define DENSITY_H
#include <base/box.h>

namespace resist {

class Density : public base::Box
{
public:
    Density(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    QVector<double> immigration, reproduction, survival;
    double densityThreshold;
    int step, maxStep;
    // Outputs
    double ss, sr, rr, total;
    QVector<double> values;
    int thresholdGen;
    bool thresholdPassed;
};

}

#endif
