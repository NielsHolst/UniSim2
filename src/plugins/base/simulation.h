#ifndef SIMULATION_H
#define SIMULATION_H
#include <boxes/box.h>

namespace base {

class Simulation : public boxes::Box
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
