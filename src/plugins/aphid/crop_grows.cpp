#include <base/publish.h>
#include "crop_grows.h"

using namespace base;

namespace aphid {

PUBLISH(Crop_grows)

Crop_grows::Crop_grows(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (Temperature);
    Input (Date);
    Input (Starting_date);
    Output (Growth);
}

void Crop_grows::reset() {
    Temperature_2=
    Temperature_3=
    Temperature_4=
    Temperature_5=-1;
    Growth=false;
}

void Crop_grows::update() {
    // I try with date here
    Growth=(Temperature>0 && Temperature_2>0 && Temperature_3>0 && Temperature_4>0 && Temperature_5>0 && Date>Starting_date) ;
    Temperature_5 = Temperature_4;
    Temperature_4 = Temperature_3;
    Temperature_3 = Temperature_2;
    Temperature_2 = Temperature;
    }
}
