#include <base/publish.h>
#include "fungus_start_simple.h"

using namespace base;

namespace aphid {

PUBLISH(Fungus_start_simple)

Fungus_start_simple::Fungus_start_simple(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (First_immigrants);
    Input (RH);
    Input (RH_threshold); // will be the flag of the wheat_start2.cpp (it is an output now)
    Input (first_infection_pressure);
    Output (First_infection);
}

void Fungus_start_simple::reset() {
    First_infection=0;
    day_count=0;
}

void Fungus_start_simple::update() {
// the first infection occur when the aphids start colonising the crop
// if the humidity is high enough.
    if (First_immigrants>0 && RH>=RH_threshold && day_count<=7)
    {First_infection=first_infection_pressure;
     day_count=day_count+1;}
    else {First_infection=0;}
    }
}
