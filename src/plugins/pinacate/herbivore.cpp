#include <base/publish.h>
#include "herbivore.h"

using namespace base;

namespace pinacate {

PUBLISH(Herbivore)

Herbivore::Herbivore(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(weight).help("weight in kg of each indv");
    Input(popsize).help("number of individuals currently alive in the specified stage");

    Output(value); //this value will be in kg herbivore
}

void Herbivore::reset() {
}

void Herbivore::update() {
//calculate the biomass of herbivore
    value = (weight*popsize);
}


}
