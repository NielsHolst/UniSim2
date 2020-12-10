#include <base/publish.h>
#include "nitrogen_uptake.h"

using namespace base;

namespace saccharina {

PUBLISH(NitrogenUptake)

NitrogenUptake::NitrogenUptake(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the relative nitrogen uptake rate (eq. 8)");
    Input(nitrogenDemand).imports("demand/nitrogenTotal[value]");
    Input(fCurrent).imports("./fCurrent[value]");
    Input(N).imports("env[N]").unit("mu mol N/L").help("Water nitrogen concentration");
    Input(KN).equals(0.004).unit("mu mol N/L").help("Half saturation constant");
    Output(supply).unit("g N").help("Nitrogen supply");
    Output(sdRatio).unit("[0;1]").help("Nitrogen Supply/Demand ratio");
}

void NitrogenUptake::reset() {
   update();
}

void NitrogenUptake::update() {
    supply = nitrogenDemand*fCurrent*N/(KN + N);
    sdRatio = (nitrogenDemand==0.) ? 0. : supply/nitrogenDemand;
}

}
