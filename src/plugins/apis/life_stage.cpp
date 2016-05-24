#include <algorithm>
#include <base/publish.h>
#include <base/vector_op.h>
#include "life_stage.h"

using namespace base;

namespace apis {

PUBLISH(LifeStage)

LifeStage::LifeStage(QString name, QObject *parent)
    : Box(name, parent),
    _number(&number),
    _mass(&mass),
    _age(&age)
{
    Input(timeStepDays).imports("calendar[timeStepDays]");
    Input(Pmorph).imports("./Pmorph[y]");
    Input(massIncrement).imports("./massIncrement[y]");
    Input(bufferSize).equals(30);
    Input(numberInit).equals(0);
    Input(massInit).equals(0);
    Input(numberNew);
    Input(massNew);
    Output(number);
    Output(mass);
    Output(age);
    Output(numberSum);
    Output(massMin);
    Output(massMax);
    Output(massAverage);
    Output(numberMorphed);
    Output(massMorphed);
    Output(numberLeaked);
    Output(massLeaked);
}

void LifeStage::initialize() {
    _number.resize(bufferSize);
    _mass.resize(bufferSize);
    _age.resize(bufferSize);
}

void LifeStage::reset() {
    _number.clear();
    _mass.clear();
    _age.clear();

    _number.push(numberInit);
    _mass.push(massInit);
    _age.push(0);
    updateStatistics();
}

void LifeStage::update() {
    // Metamorphosis
    Cohorts numberMorphedCohorts(bufferSize),
            massMorphedCohorts(bufferSize);
    vector_op::multiply(numberMorphedCohorts, number, Pmorph),
    vector_op::multiply(massMorphedCohorts, mass, numberMorphedCohorts);
    vector_op::minus(number, numberMorphedCohorts);
    numberMorphed = vector_op::sum(numberMorphedCohorts);
    massMorphed = (numberMorphed == 0.) ? 0. : vector_op::sum(massMorphedCohorts)/numberMorphed;
    // Growth
    vector_op::plus(mass, massIncrement);
    // Cohorts development
    vector_op::plus(age, timeStepDays);
    numberLeaked = _number.push(numberNew);
    massLeaked =_mass.push(massNew)*numberLeaked;
    _age.push(0);
    updateStatistics();
}

void LifeStage::updateStatistics() {
    numberSum = _number.sum();
    massMin = _mass.min();
    massMax = _mass.max();
    massAverage = _mass.average();
}

}
