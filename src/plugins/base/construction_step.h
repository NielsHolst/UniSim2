/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
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
