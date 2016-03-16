#include <base/publish.h>
#include "day_degrees.h"

using namespace base;

namespace vg {

PUBLISH(DayDegrees)

DayDegrees::DayDegrees(QString name, QObject *parent)
    : PhysiologicalTime(name, parent)
{
    Input(T0).equals(0);
    Input(Topt).equals(100);
    Input(Tmax).equals(100);
    Input(T).imports("weather[Tavg]");
}

double DayDegrees::calcDailyTimeStep()
{
    double step;
    if (T < T0)
        step = 0.;
    else if (T < Topt)
        step = T - T0;
    else if (T < Tmax)
        step = Tmax - T;
    else
        step = 0;
    return step;
}

} //namespace

