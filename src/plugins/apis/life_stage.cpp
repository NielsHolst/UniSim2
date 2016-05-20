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
    Input(numberInit).equals(1);
    Input(massInit).equals(1);
    Input(numberNew);
    Input(massNew);
    Output(number);
    Output(mass);
    Output(age);
    Output(numberMorphed);
    Output(massMorphed);
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
}

void LifeStage::update() {
    // Metamorphosis
    Cohorts numberMorphedCohorts(bufferSize),
            massMorphedCohorts(bufferSize);
    vector_op::multiply(numberMorphedCohorts, number, Pmorph),
    vector_op::multiply(massMorphedCohorts, mass, Pmorph);
    vector_op::minus(number, numberMorphedCohorts);
    vector_op::minus(mass, massMorphedCohorts);
    numberMorphed = vector_op::sum(numberMorphedCohorts);
    massMorphed = vector_op::sum(massMorphedCohorts);
    // Growth
    vector_op::plus(mass, massIncrement);
    // Cohorts development
    vector_op::plus(age, timeStepDays);
    _number.push(numberNew);
    _mass.push(massNew);
    _age.push(0);
}

}
