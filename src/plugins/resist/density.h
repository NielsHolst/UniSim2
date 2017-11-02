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
    double immigration;
    int step;
    QVector<double> genotypeFrequency, reproduction, survival;
    // Outputs
    double value;
    // Data
    double Fss, Fsr, Frr;
    // Methods
    void updateSS();
    void updateSR();
    void updateRR();
};

}

#endif
