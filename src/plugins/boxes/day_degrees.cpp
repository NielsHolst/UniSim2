#include <base/dialog.h>

#include <base/publish.h>
#include "day_degrees.h"
using namespace base;

namespace boxes {

PUBLISH(DayDegrees)

DayDegrees::DayDegrees(QString name, QObject *parent)
    : Box(name, parent) {
    help("computes linear day-degrees");
    Input(T0).equals(0).help("Lower temperature threshold");
    Input(Topt).equals(100).help("Optimum temperate; linear decline from here to Tmax");
    Input(Tmax).equals(100).help("Upper temperature threshold");
    Input(T).imports("weather[Tavg]");
    Input(scale).equals(1).help("Scaling factor applied to step");
    Input(resetTotal).equals(false).help("Reset total to zero?");
    Output(step).help("Increment in day-degrees");
    Output(total).help("Total day-degrees since reset");
}

void DayDegrees::update() {
    if (T < T0)
        step = 0.;
    else if (T < Topt)
        step = T - T0;
    else if (T < Tmax)
        step = Topt - T;
    else
        step = 0;
    step *= scale;
    if (resetTotal)
        total = 0;
    total += step;
}

} //namespace

