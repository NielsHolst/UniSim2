#include <base/publish.h>
#include "Area.h"

using namespace base;

namespace saccharina {

PUBLISH(Area)

Area::Area(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Calculates the Area after a certain timespan (dm2)");
    Input(u).equals(0.18).help("Specific growth rate");
    Input(Ainit).equals(0.000005).help("Initial Area in dm");
    Input(fronderosion).equals(0.1).help("Apical frond loss pr day");
    Input(Alost).equals(0.05).help("Area lost due to subcritical C value (dm)");
    Input(Factor).equals(1).help("Scaling factor of model");
    Output(A).help("Area of the algae at deployment (dm)");
}

void Area::reset() {
   A = Ainit;
}

void Area::update() {
   A += (A*(u-fronderosion)-Alost)*Factor;
}


}