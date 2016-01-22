#ifndef SIMULATION_H
#define SIMULATION_H
#include <base/box.h>

namespace boxes {

class Simulation : public base::Box
{
public:
    Simulation(QString name, QObject *parent);
    void run();
private:
    // Inputs
    int iterations, steps;
    // Outputs
    int iteration, step;
};

}

#endif
