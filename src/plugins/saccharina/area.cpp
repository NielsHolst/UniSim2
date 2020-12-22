#include <base/phys_math.h>
#include <base/publish.h>
#include "area.h"

using namespace base;
using phys_math::PI;

namespace saccharina {

PUBLISH(Area)

Area::Area(QString name, QObject *parent)
    : Box(name, parent)
{
    help("keeps tracks of the frond area");
    Input(aa).equals(2.72).unit("dm2").help("Power law scaling of weight to area (y=ax^b)");
    Input(ab).equals(0.75).help("Power law scaling of weight to area (y=ax^b)");
    Input(l).equals(3.5).help("Scaling of length to area");
    Input(structuralMass).imports("structure[mass]");
    Input(plantDensity).equals(1.).unit("per m line").help("No. of plants per m line");
    Input(frondAngle).unit("[0:90]").equals(30.).help("Angle of from to horizontal");
    Output(value).unit("dm2/plant").help("Frond area");
    Output(crownZoneArea).unit("m2 ground per m line").help("Crown zone area");
    Output(lai).unit("m2/m2").help("Leaf area index");
    Output(length).unit("dm").help("Frond length");
    Output(kA).unit("g structure/dm2").help("Structural dry weight per unit area");
}

void Area::reset() {
    update();
}

void Area::update() {
    double a = aa*pow(structuralMass, ab);
    length = l*sqrt(a);
    double Afrond = plantDensity*a,
           AZ = cos(frondAngle/180.*PI)*length;
    value = a;
    crownZoneArea = AZ/100.;
    lai = (AZ>0.) ? Afrond/AZ : 0.;
    kA = structuralMass/a;
}

}
