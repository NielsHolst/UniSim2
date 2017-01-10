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

