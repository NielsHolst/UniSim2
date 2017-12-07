#ifndef DAY_DEGREES
#define DAY_DEGREES
#include <base/physiological_time.h>

namespace boxes {

class DayDegrees : public base::PhysiologicalTime
{
public: 
    DayDegrees(QString name, QObject *parent);
    void reset();
    void updateStep();
private:
    // Inputs
    double T0, Topt, Tmax;
};

} //namespace
#endif
