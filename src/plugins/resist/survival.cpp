#include <base/publish.h>
#include "survival.h"

using namespace base;

namespace resist {

PUBLISH(Survival)

Survival::Survival(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(value).help("Survival after spraying [0;1]");
}

}
