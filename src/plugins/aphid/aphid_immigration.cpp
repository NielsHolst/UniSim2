#include <base/publish.h>
#include "aphid_immigration.h"

using namespace base;

namespace aphid {

PUBLISH(AphidImmigration)

AphidImmigration::AphidImmigration(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes daily immigration");
    Input(cropGrowthStage).help("Crop growth stage").unit("Zadoks");
    Input(immigrationRate).equals(15).help("Immigration rate in the season").unit("per tiller d-1");
    Input(propExposedImmigrants).help("Proportion of infected immigrants").unit("[0;1]");
    Input(k).help("Has to match k of the receiving StageAndPhase box").unit("positive int");
    Output(total).help("Total immigration rate").unit("per tiller d-1");
    Output(susceptible).help("Immigration rate of susceptible aphids").unit("per tiller d-1");;
    Output(exposed).help("Immigration rate of exposed aphids").unit("per tiller d-1");;
}

void AphidImmigration::reset() {
    exposed.resize(k);
    bool withoutFungus = findOne<Box>("..")->name().contains("withoutFungus");
    if (withoutFungus)
        propExposedImmigrants = 0.;
}

void AphidImmigration::update() {
    total = (cropGrowthStage>31 && cropGrowthStage<80) ? immigrationRate : 0.;
    susceptible = total*(1.-propExposedImmigrants);
    // Number followed by all zeros in the rest of the vector
    exposed[0] = total*propExposedImmigrants;
}

}
