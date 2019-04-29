#include <base/publish.h>
#include "threshold_base.h"

using namespace base;

namespace aphid {

ThresholdBase::ThresholdBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(cropGrowthStage).help("Crop growth stage").unit("Zadoks");
    Input(aphids).equals(0).help("Density of aphids").unit("per tiller");
    Output(exceeded).help("Was the threshold exceeded?").unit("boolean");
    Output(exceededAtGS).help("Crop growth when threshold was exceeded, or else 0").unit("Zadoks");
}

void ThresholdBase::reset() {
    exceeded = false;
    exceededAtGS = 0;
}

}
