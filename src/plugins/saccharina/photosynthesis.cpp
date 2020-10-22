#include <base/publish.h>
#include <base/test_num.h>
#include "photosynthesis.h"

using namespace base;

namespace saccharina {

PUBLISH(Photosynthesis)

Photosynthesis::Photosynthesis(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the rate of photosynthesis");
    Input(area).imports("area[value]");
    Input(lai).imports("area[lai]");
    Input(plantDensity).imports("area[plantDensity]");
    Input(k).equals(0.6).help("Light extinction coefficient");
    Input(demand).imports("demand/carbonTotal[value]");
    Input(alpha).unit("g C/mol PAR").help("Photosynthetic efficiency ");
    Input(fTemp).equals(1).unit("[0;1]").help("Temperature scaling on alpha");
    Input(fNitrogen).equals(1).unit("[0;1]").help("Nitrogen scaling on alpha");
    Input(I).imports("env[I]");
    Output(supply).unit("g C").help("Photosynthetic supply");
    Output(sdRatio).unit("[0;1]").help("Supply/Demand ratio");
    Output(Iabsorbed).unit("mu mol PAR / s").help("Radiation absorbed per plant");
}

void Photosynthesis::reset() {
   update();
}

void Photosynthesis::update() {
    const double c = 3600e-6;
    Iabsorbed = I*(1. - exp(-k*lai))/plantDensity;
    supply = TestNum::eqZero(demand) ?
                0. :
                demand*(1. - exp(-alpha*Iabsorbed*fTemp*fNitrogen*c/demand));
    sdRatio = (demand==0.) ? 0. : supply/demand;
}

}
