#include <base/publish.h>
#include "Effect_of_temperature_on_growthrate.h"

using namespace base;

namespace saccharina {

PUBLISH(Effectoftemperatureongrowthrate)

Effectoftemperatureongrowthrate::Effectoftemperatureongrowthrate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the effect of temperature on growth rate (Eq 4)");
    Input(T).equals(15).help("Water temperature, environmental variable");
    Output(ftemp).help("Unit is temperature");
}

void Effectoftemperatureongrowthrate::reset() {
   update();
}

void Effectoftemperatureongrowthrate::update() {
    if (T < 10)
    {
        ftemp = 0.08*T + 0.2;
    }
    else if ((T >= 10) && (T <= 15))
    {
        ftemp = 1;
        }
    else if ((T > 15) && (T <= 19))
        {
        ftemp = (19.0/4.0) - (T/4.0);
        }
    else
        {
        ftemp = 0;
        }
}
}
