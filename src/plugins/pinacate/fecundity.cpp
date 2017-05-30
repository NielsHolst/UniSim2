#include <base/publish.h>
#include "fecundity.h"

using namespace base;

namespace pinacate {

PUBLISH(Fecundity)

Fecundity::Fecundity(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(density).help("number of adults currently alive");
    Input(percapita).help("reproductive output of each adult");
    Input(availability).help("vegetation availability limits population growth");

    Output(value);
}

void Fecundity::reset() {
}

void Fecundity::update() {
//calculate the number of new offspring produced
    value = density*percapita*availability;
}
}
