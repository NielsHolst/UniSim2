#ifndef POPULATION_H
#define POPULATION_H
#include <base/box.h>
#include <base/circular_buffer.h>

namespace boxes {

class Population : public base::Box
{
public:
    Population(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double initial, newBorn, ageIncrement;
    QVector<double> transferred;
    int bufferSize;
    bool debugMode;
    // Outputs
    QVector<double> cohort, ageIncrements;
    // Data
    base::CircularBuffer<double> _cohort, _ageIncrements;
};

}

#endif
