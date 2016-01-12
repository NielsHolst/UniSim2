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
    double initialDensity, growthRate,
        preyDensity, searchRate, demand;
    // Outputs
    double density, killRate;
};

}

#endif
