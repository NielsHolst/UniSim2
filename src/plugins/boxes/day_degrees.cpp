#include <base/exception.h>
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
    Input(timeStepDays).equals(1).help("Length of a time steps in days");
    Input(resetTotal).equals(false).help("Reset total to zero?");
    Input(isTicking).equals(true).help("Are day-degrees being computed? Else step will be zero");
    Output(step).help("Increment in day-degrees");
    Output(total).help("Total day-degrees since reset");
}

void DayDegrees::reset() {
    if (T0 >= Topt) {
        QString msg{"T0 (%1) must be smaller than Topt (%2)"};
        ThrowException(msg.arg(T0).arg(Topt)).context(this);
    }
    if (Topt > Tmax) {
        QString msg{"Topt (%1) cannot be larger than Tmax (%2)"};
        ThrowException(msg.arg(Topt).arg(Tmax)).context(this);
    }
}

void DayDegrees::update() {
    if (!isTicking)
        step = 0.;
    else if (T <= T0)
        step = 0.;
    else if (T <= Topt)
        step = T - T0;
    else if (T >= Tmax)
        step = 0;
    else
        step = (Topt - T0)*(Tmax - T)/(Tmax - Topt);
    step *= timeStepDays;
    if (resetTotal)
        total = 0;
    total += step;
}

} //namespace

