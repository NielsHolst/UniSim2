#ifndef SIMULATION_TIME_INTERVAL_H
#define SIMULATION_TIME_INTERVAL_H

#include <QDate>
#include <QTime>
#include <base/box.h>

namespace vg {
	
class SimulationTimeInterval : public base::Box
{
public:
    SimulationTimeInterval(QString name, QObject *parent);
    void reset();
private:
    // Input
    QDate beginDate, endDate;
    QTime beginTime, endTime;
    int timeStep;
    char timeUnit;
    // Output
    QDateTime beginDateTime;
    int steps;
    // Methods
    long long int secsInterval();
    long long int secsTimeStep();
};

} //namespace

#endif
