#include "computation_step.h"

namespace base {

#define CASE_COMPUTATION_STEP(x) case ComputationStep::x: return #x

template<> QString convert(ComputationStep step) {
    switch (step) {
        CASE_COMPUTATION_STEP(Start);
        CASE_COMPUTATION_STEP(Ready);
        CASE_COMPUTATION_STEP(Construct);
        CASE_COMPUTATION_STEP(Amend);
        CASE_COMPUTATION_STEP(Initialize);
        CASE_COMPUTATION_STEP(Reset);
        CASE_COMPUTATION_STEP(Update);
        CASE_COMPUTATION_STEP(Cleanup);
        CASE_COMPUTATION_STEP(Debrief);
    }
    return QString();
}

}

