#include <base/publish.h>
#include "humidity_switch.h"

using namespace base;

namespace aphid {

PUBLISH(Humidity_switch)

Humidity_switch::Humidity_switch(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (RH_threshold);
    Input (RH);
    Output(RH_switch);
}

void Humidity_switch::reset() {
    RH_switch=false;
}

void Humidity_switch::update() {
    if(RH>=RH_threshold) {RH_switch=true;}
    if(RH<RH_threshold){RH_switch=false;}
}
}
