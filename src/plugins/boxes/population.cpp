#include <base/exception.h>
#include <base/publish.h>
#include "population.h"

using namespace base;

namespace boxes {

PUBLISH(Population)

Population::Population(QString name, QObject *parent)
    : Box(name, parent),
      _cohort(&cohort),
      _ageIncrements(&ageIncrements)
{
    Input(initial).equals(100);
    Input(newBorn);
    Input(transferred);
    Input(bufferSize).equals(100);
    Input(ageIncrement);
    Input(debugMode).equals(false);
    Output(cohort);
    Output(ageIncrements);
}

void Population::reset() {
    _cohort.resize(bufferSize);
    _ageIncrements.resize(bufferSize);

    _cohort.push(initial);
    _ageIncrements.push(0);
}

void Population::update() {
    _cohort.push(newBorn);
    _ageIncrements.push(ageIncrement);
}



}
