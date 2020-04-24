#include <base/publish.h>
#include "Temperature_dependent_respiration.h"

using namespace base;

namespace saccharina {

PUBLISH(Temperaturedepententrespiration)

Temperaturedepententrespiration::Temperaturedepententrespiration(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the respiration (Eq 14)");
    Input(R1).equals(0.0002785).help("Respiration rate at T = TR1");
    Input(TAR).equals(11033).help("Arrhenius temperature for respiration");
    Input(TR1).equals(285).help("Reference temperature for respiration");
    Input(T).equals(15).help("Water temperature, environmental variable");
    Output(respiration).help("Temperature dependent respiration");
}

void Temperaturedepententrespiration::reset() {
   update();
}

void Temperaturedepententrespiration::update() {
    respiration = R1 * exp((TAR/TR1) - (TAR/(T+273.15)));
}
}
