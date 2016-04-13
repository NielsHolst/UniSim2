#ifndef POPULATION_H
#define POPULATION_H
#include <base/box.h>
#include <base/circular_buffer.h>
#include <base/port.h>

namespace boxes {

class Population : public base::Box
{
public:
    Population(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double initial, ageIncrement;
    int bufferSize;
    double newBorn;
    QVector<double> gains, losses;
    // Outputs
    QVector<double> cohorts, ageIncrements;
    // Data
    base::CircularBuffer<double> _cohorts, _ageIncrements;
    QVector<base::Port*> _gains, _losses, _newBorns;
};

}

#endif
