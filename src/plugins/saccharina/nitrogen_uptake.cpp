#include <base/publish.h>
#include "nitrogen_uptake.h"

using namespace base;

namespace saccharina {

PUBLISH(NitrogenUptake)

// 1 umol N = 14 ug N

NitrogenUptake::NitrogenUptake(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the nitrogen uptake rate");
    Input(beta).equals(10.).unit("g N/(umol N/L)/h").help("Effiency of nitrogen uptake");
    Input(demand).imports("demand/nitrogenTotal[value]");
    Input(area).imports("area[value]");
    Input(lai).imports("area[lai]");
    Input(fCurrent).imports("./fCurrent[value]");
    Input(N).imports("env[N]").unit("umol N/L").help("Water nitrogen concentration");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Output(supply).unit("g N").help("Nitrogen supply");
    Output(sdRatio).unit("[0;1]").help("Nitrogen Supply/Demand ratio");
    Output(J).unit("g N/dm2/h").help("J of Broch et al. (2011)");
}

void NitrogenUptake::reset() {
   update();
}

void NitrogenUptake::update() {
    double dt = timeStepSecs/3600.;
    supply = (demand<1e-16) ?
                0. :
                demand*(1. - exp(-beta*lai*N*fCurrent*dt/demand));
    sdRatio = (demand==0.) ? 0. : supply/demand;
    J = supply/area/dt;
}

}
