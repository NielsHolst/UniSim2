#include "physiological_time.h"

namespace base {

PhysiologicalTime::PhysiologicalTime(QString name, QObject *parent)
    : Box(name, parent) {
    Class(PhysiologicalTime);
    Input(T).imports("weather[Tavg]");
    Input(timeStepDays).equals(1).help("Length of a time steps in days");
    Input(resetTotal).equals(false).help("Reset total to zero?");
    Input(isTicking).equals(true).help("Are day-degrees being computed? Else step will be zero");
    Output(step).help("Increment in day-degrees");
    Output(total).help("Total day-degrees since reset");
}

void PhysiologicalTime::update() {
    if (!isTicking)
        step = 0.;
    else
        updateStep();
    if (resetTotal)
        total = 0;
    total += step;
}

} //namespace


