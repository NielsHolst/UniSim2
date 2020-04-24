#include <base/publish.h>
#include "Seasonal_influence_on_growthrate.h"

using namespace base;

namespace saccharina {

PUBLISH(Seasonalinfluenceongrowthrate)

Seasonalinfluenceongrowthrate::Seasonalinfluenceongrowthrate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the seasonal influence on growth rate (Eq 5)");
    Input(a1).equals(0.85).help("Photoperiodic parameter");
    Input(a2).equals(0.3).help("Photoperiodic parameter");
    Input(n).equals(365).help("day number");
    Output(fphoto).help("Unit is temperature");
}

void Seasonalinfluenceongrowthrate::reset() {
   update();
}

void Seasonalinfluenceongrowthrate::update() {
       if (tgamma(n) > 0.0)
       {
       fphoto = a1 * (pow(abs(tgamma(n)),0.5)) + a2;
            }
       if (tgamma(n) < 0.0)
            {
            fphoto = a1 * (-1 * (pow(abs(tgamma(n)),0.5))) + a2;
                 }
        else
            {
           fphoto = a2;
                  }
 }
}
