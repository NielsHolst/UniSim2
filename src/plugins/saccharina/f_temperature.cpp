#include <base/publish.h>
#include "f_temperature.h"

using namespace base;

namespace saccharina {

PUBLISH(Ftemperature)

Ftemperature::Ftemperature(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the effect of temperature on growth rate (eq. 4)");
    Input(T).imports("env[T]").help("Water temperature");
    Output(value).unit("[0;1]").help("Growth factor adjustment for temperature");
}

void Ftemperature::reset() {
   update();
}

void Ftemperature::update() {
    if (T < 10)
        value = 0.08*T + 0.2;
    else if (T < 15)
        value = 1;
    else if (T < 19)
        value = (19.0/4.0) - (T/4.0);
    else
        value = 0;
    if (value < 0.)
        value = 0.;
}

}
