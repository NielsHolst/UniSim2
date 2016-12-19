#include <base/publish.h>
#include "wheat_start.h"

using namespace base;

namespace student {

PUBLISH(Wheat_start)

Wheat_start::Wheat_start(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (Temperature);
    Input (Nb_plants);
    Output(startPlants);
}

void Wheat_start::reset() {
    startPlants = 0;
    Temperature_2=
    Temperature_3=
    Temperature_4=
    Temperature_5=-1;
    growthAlreadyStarted=false;
}

void Wheat_start::update() {
    if (growthAlreadyStarted) {
        startPlants=0;
    }
    else {
        growthAlreadyStarted = (Temperature>0 && Temperature_2>0 && Temperature_3>0 && Temperature_4>0 && Temperature_5>0);
        if (growthAlreadyStarted)
            startPlants=Nb_plants;
        else {
            Temperature_5 = Temperature_4;
            Temperature_4 = Temperature_3;
            Temperature_3 = Temperature_2;
            Temperature_2 = Temperature;
        }
    }
}

}
