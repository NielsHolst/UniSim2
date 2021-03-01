/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_COMPUTATION_STEP_H
#define BASE_COMPUTATION_STEP_H

#include "exception.h"

namespace base {

enum class ComputationStep {Start, Ready, Construct, Amend, Initialize, Reset, Update, Cleanup, Debrief};

template<class T> T convert(ComputationStep) {
    ThrowException("Cannot convert ComputationStep");
    return T();
}

template<> QString convert(ComputationStep step);

}
#endif

