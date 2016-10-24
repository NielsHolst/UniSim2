#include <base/publish.h>
#include "growth_factor.h"

using namespace base;

namespace tuta_absoluta {

PUBLISH(GrowthFactor)

GrowthFactor::GrowthFactor(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(sexRatio).equals(75).help("Ratio of females in the population(%)");
    Input(eggsPerFemale).equals(75).help("Number of eggs laid per female");
    Output(growth_Factor).help("Is multiplied by the current population to get eggs laid");
}

void GrowthFactor::reset() {
    growth_Factor = 0.;
}

void GrowthFactor::update() {
    growth_Factor = (sexRatio/100.) * eggsPerFemale;
}


}
