#include <base/publish.h>
#include "area.h"

using namespace base;

namespace saccharina {

PUBLISH(Area)

Area::Area(QString name, QObject *parent)
    : Box(name, parent)
{
    help("keeps tracks of the frond area");
    Input(aa).equals(2.72).help("Power law scaling of weight to area (y=ax^b)");
    Input(ab).equals(0.75).help("Power law scaling of weight to area (y=ax^b)");
    Input(l).equals(3.5).help("Scaling of length to area");
    Input(structuralMass).imports("structure[mass]");
    Input(plantDensity).equals(1.).unit("per m2").help("Plant density");
    Output(value).unit("dm2").help("Frond area");
    Output(lai).unit("m2/m2").help("Leaf area index");
    Output(kA).unit("g structure/dm2").help("Structural dry weight per unit area");
}

void Area::reset() {
    update();
}

void Area::update() {
    const double c = 2.*sqrt(0.75);
    value = aa*pow(structuralMass, ab);
    length = l*sqrt(value);
    lai = plantDensity*sqrt(value)/c/l;
    kA = structuralMass/value;
}

}
