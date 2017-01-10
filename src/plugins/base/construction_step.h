#ifndef BASE_CONSTRUCTION_STEP_H
#define BASE_CONSTRUCTION_STEP_H

#include "computation_step.h"

namespace base {

class ConstructionStep
{
public:
    ConstructionStep();
    ComputationStep constructionStep() const;
    bool underConstruction() const;
    void finishConstruction();
private:
    ComputationStep _constructionStep;
    bool _underConstruction;
};

}
#endif
