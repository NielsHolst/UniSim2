#include <base/publish.h>
#include "immigrant_oviposition.h"

using namespace base;

namespace bph {

PUBLISH(ImmigrantOviposition)

ImmigrantOviposition::ImmigrantOviposition(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes oviposition rate of immigrant BPH");
    Input(immigrants).imports("environment[Immigrants]").help("Number of immigrants present");
    Input(vegetative).imports("environment[Vegetative]");
    Input(fecundity).help("Daily no. of eggs laid per immigrant");
    Output(value).help("Daily no. of eggs laid by immigrants");
}

void ImmigrantOviposition::reset() {
    update();
}

void ImmigrantOviposition::update() {
    // Immigrants are only present when rice is strictly in the vegetative stage;
    // i.e. they are not present leading or trailing the growing season
    // value = (vegetative==1) ? immigrants*fecundity : 0.;
    value = immigrants*fecundity;
}


}
