#include <base/publish.h>
#include "Salinity.h"

using namespace base;

namespace saccharina {

PUBLISH(Salinity)

Salinity::Salinity(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the effect of salinity on growth rate (factor)");
    Input(S).equals(25).help("Environmental salinity level (PSU)");
    Output(fsalinity).help("Effect of salinity of growthrate (factor)");
}

void Salinity::reset() {
   update();
}

void Salinity::update() {
    if (S >= 25)
    {
        fsalinity = 1;
    }
    else if ((S >= 16) && (S <= 25))
    {
        fsalinity = 1 + ((S - 25)/18);
        }
    else
        {
        fsalinity = S/32;
        }
}
}

