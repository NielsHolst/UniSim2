#include <cmath>
#include <base/publish.h>
#include "aphid_juvenile_survival.h"

using std::max;
using namespace base;

namespace aphid {

PUBLISH(AphidJuvenileSurvival)

AphidJuvenileSurvival::AphidJuvenileSurvival(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes juvenile survival");
    Input(cropGrowthStage).help("Crop growth stage").unit("Zadoks");
    Input(temperature).help("Daily average temperature").unit("oC");
    Output(value).help("Juvenile survival").unit("[0;1] d-1");
}

void AphidJuvenileSurvival::reset() {
    update();
}

void AphidJuvenileSurvival::update() {
    if(cropGrowthStage < 73.)
        value = max(0.944 - 3.32e-10*exp(0.726*temperature), 0.);
    else if (cropGrowthStage < 80.)
        value = 0.45;
    else
        value = 0.;
}

}
