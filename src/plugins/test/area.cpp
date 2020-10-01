#include <base/publish.h>
#include "area.h"

using namespace base;

namespace test {

PUBLISH(Area)

Area::Area(QString name, QObject *parent)
    : Box(name, parent)
{
}

}
