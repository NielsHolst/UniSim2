#include "construction_step.h"
#include "environment.h"

namespace base {

ConstructionStep::ConstructionStep()
{
    _underConstruction = true;
    _constructionStep = environment().computationStep();
}

bool ConstructionStep::underConstruction() const {
    return _underConstruction;
}

void ConstructionStep::finishConstruction() {
    _underConstruction = false;
}

ComputationStep ConstructionStep::constructionStep() const {
    return _constructionStep;
}

}
