#include <base/publish.h>
#include "wheat_start2.h"

using namespace base;

namespace aphid {

PUBLISH(Wheat_start2)

Wheat_start2::Wheat_start2(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (Temperature);
    Input (Nb_plants);
    Input (Date);
    Output(startPlants);
    Output (growthAlreadyStarted);
}

void Wheat_start2::reset() {
    startPlants = 0;
    Temperature_2=
    Temperature_3=
    Temperature_4=
    Temperature_5=-1;
    growthAlreadyStarted=false;
}

void Wheat_start2::update() {
    if (growthAlreadyStarted) {
        startPlants=0;
    }
    else {
        growthAlreadyStarted = (Temperature>0 && Temperature_2>0 && Temperature_3>0 && Temperature_4>0 && Temperature_5>0 && Date>90);
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
