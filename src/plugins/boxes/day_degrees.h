#ifndef DAY_DEGREES
#define DAY_DEGREES
#include <base/box.h>

namespace boxes {

class DayDegrees : public base::Box
{
public: 
    DayDegrees(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double T0, Topt, Tmax, T, timeStepDays;
    bool resetTotal, isTicking;
    // Outputs
    double step, total;
};

} //namespace
#endif
