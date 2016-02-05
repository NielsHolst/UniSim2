#ifndef BOX_STEP_H
#define BOX_STEP_H
#include "convert.h"

namespace base {

    enum Step {Amend=1, Initialize=2, Reset=4, Update=8, Cleanup=16, Debrief=32};

    template<> Step convert(QString source);

    QString convert(Step source);
}

#endif

