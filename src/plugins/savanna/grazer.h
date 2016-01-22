#ifndef GRAZER_H
#define GRAZER_H
#include <base/box.h>

namespace savanna {

class Grazer : public base::Box
{
public:
    Grazer(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double initialDensity, growthRate, killRate, carryingCapacity;
    // Outputs
    double density;
};

}

#endif
