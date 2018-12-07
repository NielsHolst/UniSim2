#include <base/publish.h>
#include "potato_development_base.h"

using namespace base;

namespace potato {

PotatoDevelopmentBase::PotatoDevelopmentBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(timeStepDays).imports("calendar[timeStepDays]");
    Output(step).help("The daily increment in the physiologic age of the potato");
    Output(total).help ("The accumulated daily physiologic age of the potato");
}

}
