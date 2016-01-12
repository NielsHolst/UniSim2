#ifndef GRAZER_H
#define GRAZER_H
#include <boxes/box.h>

namespace savanna {

class Grazer : public boxes::Box
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
