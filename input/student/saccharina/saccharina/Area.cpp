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
    Input(dt).equals(1.0).help("timestep (days)");
    Input(Ainit).equals(0.000005);
    Output(A).help("Area of the algae at deployment (dm)");
}

void Area::reset() {
   A = Ainit;
}

void Area::update() {
    A += A*u*((dt/3600)/24);
}


}
