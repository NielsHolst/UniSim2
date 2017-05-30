#include <base/publish.h>
#include "Birth.h"

using namespace base;

namespace pinacate {

PUBLISH(Birth)

Birth::Birth(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(birthday).help("the Julian date on which cohort is born");
    Input(fecundity).help("reproductive output of adults");
    Input(currentday).help("the current day in the simulation");

    Output(value);
}

void Birth::reset() {
}

void Birth::update() {
// the birth-pulse of new juveniles into the model
    if (currentday == birthday) value = fecundity;
    else value = 0;
}


}
