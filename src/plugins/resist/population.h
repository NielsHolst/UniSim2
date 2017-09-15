#ifndef POPULATION_H
#define POPULATION_H
#include <base/box.h>

namespace resist {

class Population : public base::Box
{
public:
    Population(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Ninit, K, R, survival, Ntotal;
    int steps;
    // Outputs
    double N;
};

}

#endif
