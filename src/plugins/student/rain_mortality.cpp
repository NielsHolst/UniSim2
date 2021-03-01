#include <base/publish.h>
#include "rain_mortality.h"

using namespace base;

namespace student {

PUBLISH(RainMortality)

RainMortality::RainMortality(QString name, QObject *parent)
    : Box(name, parent)
{
    help("mortality caused by rain per day");
    Input(beta).help("Mortality caused by rain").unit("per (mm/day) per (DD/day)");
    Input(rain).help("Rain").unit("mm/day");
    Input(timeStep).help("Time step").unit("DD/day");
    Output(value).help("Mortality").unit("per day");
}

void RainMortality::reset() {
    update();
}

void RainMortality::update(){
    value = 1 - exp(-beta*rain*timeStep);
 }



}
