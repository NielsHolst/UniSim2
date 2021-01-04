#include <base/phys_math.h>
#include <base/publish.h>
#include "photosynthesis.h"

using namespace base;
using phys_math::PI;

namespace saccharina {

PUBLISH(Photosynthesis)

Photosynthesis::Photosynthesis(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the rate of photosynthesis");
    Input(crownZoneArea).imports("area[value]");
    Input(lai).imports("area[lai]");
    Input(frondAngle).imports("area[frondAngle]");
    Input(plantDensity).imports("area[plantDensity]");
    Input(demand).imports("demand/carbonTotal[value]");
    Input(alpha).unit("g C/mol PAR").help("Photosynthetic efficiency ");
    Input(fTemp).equals(1).unit("[0;1]").help("Temperature scaling on alpha");
    Input(fNitrogen).equals(1).unit("[0;1]").help("Nitrogen scaling on alpha");
    Input(I).imports("env[I]");
    Input(propI).equals(1.).help("Proportion of I reaching the plant");
    Output(supply).unit("g C").help("Photosynthetic supply");
    Output(sdRatio).unit("[0;1]").help("Supply/Demand ratio");
    Output(Iabsorbed).unit("mu mol PAR / s").help("Radiation absorbed per plant");
}

void Photosynthesis::reset() {
   update();
}

void Photosynthesis::update() {
    const double c = 3600e-6,
                 k = cos(frondAngle/180.*PI);
    Iabsorbed = propI*I*crownZoneArea/plantDensity*(1. - exp(-k*lai));
    supply = (demand<1e-16) ?
                0. :
                demand*(1. - exp(-alpha*Iabsorbed*fTemp*fNitrogen*c/demand));
    sdRatio = (demand==0.) ? 0. : supply/demand;
}

}
