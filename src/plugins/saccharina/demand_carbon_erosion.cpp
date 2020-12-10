#include <base/publish.h>
#include "demand_carbon_erosion.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandCarbonErosion)

DemandCarbonErosion::DemandCarbonErosion(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates area loss by erosion");
    Input(A).imports("area[value]");
    Input(e50).equals(1.4).unit("dm2").help("Area at which erosion reaches 50%");
    Input(eSlope).equals(-10.).help("Slope at A50");
    Input(eMax).equals(0.0003).unit("h-1").help("Max. erosion rate");
    Input(proportionC).imports("carbon/structure[proportionC]");
    Input(kA).imports("area[kA]");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Output(value).unit("g C").help("Area lost");
}

void DemandCarbonErosion::reset() {
   update();
}

void DemandCarbonErosion::update() {
    double
        rate = (A == 0.) ? 0. : eMax/(1. + exp(eSlope*(log(A) - log(e50)))),
        dt = timeStepSecs/3600.,
        lostA = A*rate*dt;
    value = proportionC*kA*lostA;
}

}
