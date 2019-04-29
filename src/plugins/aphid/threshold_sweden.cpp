#include <base/publish.h>
#include "threshold_sweden.h"

using namespace base;

namespace aphid {

PUBLISH(ThresholdSweden)

ThresholdSweden::ThresholdSweden(QString name, QObject *parent)
    : ThresholdBase(name, parent)
{
    help("evaluates aphid damage threshold used in Sweden (high yielding wheat, Larsson 2005");
}

void ThresholdSweden::update() {
    const double &GS(cropGrowthStage);
    if (!exceeded) {
        if (GS<41)
            exceeded = false;
        else if (GS<49)
            exceeded = (aphids > 1);
        else if (GS<69) //
            exceeded = (aphids > 4);
        else if (GS<82)
            exceeded = (aphids > 7);
        else
            exceeded = false;
        if (exceeded)
            exceededAtGS = GS;
    }
}

}
