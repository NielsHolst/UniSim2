#include <cmath>
#include <base/publish.h>
#include "aphid_offspring.h"

using namespace base;

namespace aphid {

PUBLISH(AphidOffspring)

AphidOffspring::AphidOffspring(QString name, QObject *parent)
    : Box(name, parent)
{
    help("splits new offspring into apterous and alate morphs");
    Input(aphidOffspring).help("Total no. of offspring produced").unit("per tiller");
    Input(aphidDensity).help("Aphid density").unit("per tiller");
    Input(cropGrowthStage).help("Crop growth stage").unit("Zadoks");
    Output(apterous).help("Total no. of apterous offspring produced").unit("per tiller");
    Output(alate).help("Total no. of alate offspring produced").unit("per tiller");
    Output(alateProportion).help("Proportion of alate offspring").unit("[0;1]");
}

void AphidOffspring::update() {
    alateProportion = (2.603*aphidDensity + 0.847*cropGrowthStage - 27.189)/100.;
    if (alateProportion<0.)
        alateProportion=0.;
    else if (alateProportion>1.)
        alateProportion = 1.;
    apterous = (1. - alateProportion)*aphidOffspring;
    alate = alateProportion*aphidOffspring;
}


}
