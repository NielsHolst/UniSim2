#include <base/publish.h>
#include "carrying_capacity.h"

using namespace base;

namespace resist {

PUBLISH(CarryingCapacity)

CarryingCapacity::CarryingCapacity(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(value).help("The carrying capacity for all genotypes total");
}

}
