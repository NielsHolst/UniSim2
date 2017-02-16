#include <base/publish.h>
#include "growthRate.h"
#include <base/exception.h>

using namespace base;

namespace aphid {

PUBLISH(GrowthRate)

GrowthRate::GrowthRate(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (growth_rate_season);
    Input (growth_rate_senescence);
    Input (proportion_senescence);
    Output(growthRate_t);
}

void GrowthRate::reset() {
    growthRate_t=0;
}

void GrowthRate::update() {
    if (proportion_senescence < 0)
       ThrowException("Negative density").value(proportion_senescence).context(this);
    else if (proportion_senescence < 1e-6)
      growthRate_t=growth_rate_season;
    else
      growthRate_t=growth_rate_senescence;
}


}
