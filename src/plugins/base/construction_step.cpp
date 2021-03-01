/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
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
