#include <base/publish.h>
#include "survival.h"

using namespace base;

namespace potato {

PUBLISH(Survival)

Survival::Survival(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(senescence).imports("potato/tuberization[outflowTotal]");
    Output(value).help("Life time survival [0;1]");
}

void Survival::reset() {
    update();
}

void Survival::update() {
    value =  1. - senescence;
}

}
