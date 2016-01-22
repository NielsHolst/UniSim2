#ifndef LION_H
#define LION_H
#include <base/box.h>

namespace savanna {

class Lion : public base::Box
{
public:
    Lion(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    int n;
    double initialDensity, growthRate,
        preyDensity, searchRate, demand;
    // Outputs
    double density, killRate;
};

}

#endif
