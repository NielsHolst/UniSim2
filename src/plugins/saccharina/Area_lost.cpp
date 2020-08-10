#include <base/publish.h>
#include "Area_lost.h"

using namespace base;

namespace saccharina {

PUBLISH(Arealost)

Arealost::Arealost(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the Area lost if C<Cmin (dm2)");
    Input(A).equals(6).help("Actual Area in dm");
    Input(C).equals(0.01).help("Carbon content (g C (g sw)^-1)");
    Input(Cmin).equals(0.01).help("Minimum carbon content (g C (g sw)^-1)");
    Input(Cstruct).equals(0.2).help("Structural bound carbon content (g C (g sw)^-1)");
    Output(Alost).help("Area of the algae at deployment (dm)");
}

void Arealost::reset() {
   update();
}

void Arealost::update() {
    if (C<Cmin) {
           Alost = A*(Cmin-C)/Cstruct;
}
    else {
        Alost = 0;
    }
}

}
