#include "box_step.h"
#include "exception.h"

#define IF_STEP(x) if (source == #x) return x;
#define CASE_STEP(x) case x: return #x;

namespace base {

    template<> Step convert(QString source) {
        IF_STEP(Amend)
        IF_STEP(Initialize)
        IF_STEP(Reset)
        IF_STEP(Update)
        IF_STEP(Cleanup)
        IF_STEP(Debrief)
        ThrowException("Cannot convert string to step").value(source);
    }

    QString convert(Step source) {
        switch(source) {
            CASE_STEP(Amend)
            CASE_STEP(Initialize)
            CASE_STEP(Reset)
            CASE_STEP(Update)
            CASE_STEP(Cleanup)
            CASE_STEP(Debrief)
        }
        return QString();
    }
}


