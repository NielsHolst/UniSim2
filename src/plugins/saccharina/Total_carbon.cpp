#include <base/publish.h>
#include "Total_carbon.h"

using namespace base;

namespace saccharina {

PUBLISH(Totalcarbon)

Totalcarbon::Totalcarbon(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the dryweight of the algae from the Area produced (g)");
    Input(C).equals(0.01).help("Carbon reserves (g C (gsw)^-1)");
    Input(Cstruct).equals(0.2).help("Amount of structurally bound carbon");
    Input(Ws).help("Structural dryweight of the algae (g)");
    Output(Ctotal).help("Total content of Carbon in each algae (g)");
}

void Totalcarbon::reset() {
   update();
}

void Totalcarbon::update() {
   Ctotal = (C + Cstruct)*Ws;
}


}
