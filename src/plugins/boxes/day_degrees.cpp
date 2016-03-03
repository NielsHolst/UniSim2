#include <base/publish.h>
#include "day_degrees.h"
using namespace base;

namespace boxes {

PUBLISH(DayDegrees)

DayDegrees::DayDegrees(QString name, QObject *parent)
    : Box(name, parent) {
    Input(T0).equals(0);
    Input(Topt).equals(100);
    Input(Tmax).equals(100);
    Input(T).imports("weather[Tavg]");
    Output(step);
    Output(total).trackOff();
}

void DayDegrees::update() {
    if (T < T0)
        step = 0.;
    else if (T < Topt)
        step = T - T0;
    else if (T < Tmax)
        step = Tmax - T;
    else
        step = 0;
    total += step;
}

} //namespace

