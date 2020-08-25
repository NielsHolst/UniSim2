#include <base/publish.h>
#include "Total_nitrogen.h"

using namespace base;

namespace saccharina {

PUBLISH(Totalnitrogen)

Totalnitrogen::Totalnitrogen(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the dryweight of the algae from the Area produced (g)");
    Input(N).equals(0.01).help("Nitrogen reserve (g N (gsw)^-1))");
    Input(Nstruct).equals(0.01).help("Amount of nitrogen per unit dry weight of structural mass");
    Input(Ws).help("Structural dryweight of the algae (g)");
    Output(Ntotal).help("Dryweight of the algae (g)");
}

void Totalnitrogen::reset() {
   update();
}

void Totalnitrogen::update() {
   Ntotal = (N + Nstruct)*Ws;
}


}
