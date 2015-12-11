#ifndef LION_H
#define LION_H
#include <boxes/box.h>

namespace savanna {

class Lion : public boxes::Box
{
public:
    Lion(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double initialDensity, growthRate, carryingCapacity;
    // Outputs
    double density;
};

}

#endif
