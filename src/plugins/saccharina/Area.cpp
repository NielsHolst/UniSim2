#include <base/publish.h>
#include "area.h"

using namespace base;

namespace saccharina {

PUBLISH(Area)

Area::Area(QString name, QObject *parent)
    : Box(name, parent)
{
    help("keeps tracks of the frond area");
    Input(structuralMass).imports("structure[mass]");
    Input(kA).equals(0.6).unit("g structure/dm2").help("Structural dry weight per unit area");
    Input(plantDensity).equals(1.).unit("per m2").help("Plant density");
    Output(value).unit("dm2").help("Frond area");
    Output(lai).help("Leaf area index");
}

void Area::reset() {
    update();
}

void Area::update() {
    value = structuralMass/kA;
    lai = plantDensity*value/100.;
}

}
