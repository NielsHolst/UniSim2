#include <base/publish.h>
#include "egg_development.h"

using namespace base;

namespace student {

PUBLISH(EggDevelopment)

EggDevelopment::EggDevelopment(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(fromDate);
    Input(toDate);
    Input(currentDate);
    Output(on);
}

void EggDevelopment::reset() {
    on = false;
}

void EggDevelopment::update() {
    on = (currentDate >= fromDate && currentDate <= toDate);
}


}
