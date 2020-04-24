#include <base/publish.h>
#include "Specific_growth_rate.h"

using namespace base;

namespace saccharina {

PUBLISH(Specificgrowthrate)

Specificgrowthrate::Specificgrowthrate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the specific growthrate (Eq 2)");
    Input(farea).equals(0.03).help("Effect of size on growth rate");
    Input(fphoto).equals(0.3).help("Seasonal influence on growth rate");
    Input(ftemp).equals(6.0).help("Effect of temperature on growth rate");
    Input(N).equals(0.01).help("Nitrogen reserve, relative to Ws, state variable");
    Input(Nmin).equals(0.01).help("Minimal nitrogen reserve");
    Input(C).equals(0.01).help("Carbon reserves, relative to Ws, state variable");
    Input(Cmin).equals(0.01).help("Minimal carbon reserve");
    Output(specificgrowthrate).help("Specific growthrate");
}

void Specificgrowthrate::reset() {
   update();
}

void Specificgrowthrate::update() {
    specificgrowthrate = farea * fphoto * ftemp * std::min(((1-Nmin)/N), ((1-Cmin)/C));
}
}
