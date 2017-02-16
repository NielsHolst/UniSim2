#include <base/publish.h>
#include "aphid_start_simple.h"

using namespace base;

namespace aphid {

PUBLISH(Aphid_start_simple)

Aphid_start_simple::Aphid_start_simple(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (Temperature);
    Input (Temperature_threshold);
    Input (Start_immigration); // is the flag of the wheat_start2.cpp
    Input (Potential_immigrants);
    Input (Immigration_duration);
    Output (First_immigrants);
}

void Aphid_start_simple::reset() {
    First_immigrants=0;
    day_count=0;
}

void Aphid_start_simple::update() {
   // the first immigrants arrive when the wheat start growing
    if (Start_immigration && Temperature>=Temperature_threshold && day_count<Immigration_duration)
        {First_immigrants=Potential_immigrants;
        day_count=day_count+1;}
    else {First_immigrants=0;}
}
}
