#ifndef LIFE_STAGE_H
#define LIFE_STAGE_H
#include <base/box.h>
#include <base/circular_buffer.h>

namespace apis {

class LifeStage : public base::Box
{
public:
    LifeStage(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    typedef QVector<double> Cohorts;
    // input
    int bufferSize;
    double timeStepDays, numberInit, massInit, numberNew, massNew;
    Cohorts Pmorph, massIncrement;
    // output
    Cohorts number, mass, age;
    double numberSum, massMin, massMax, massAverage,
        numberMorphed, massMorphed,
        numberLeaked, massLeaked;
    // data
    base::CircularBuffer<double> _number, _mass, _age;
    // methods
    void updateStatistics();
};

}

#endif
