#include <base/publish.h>
#include "fecundity.h"

using namespace base;

namespace potato {

PUBLISH(Fecundity)

Fecundity::Fecundity(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(R0).equals(170).help("Life time net fecundity [eggs per female]");
    Input(senescence).imports("potato/tuberization[outflowTotal]");
    Output(value).help("Life time fecundity [0;1]");
}

void Fecundity::reset() {
    update();
}

void Fecundity::update() {
    value = R0*(1. - senescence);
}

}
