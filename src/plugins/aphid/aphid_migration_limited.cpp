#include <base/publish.h>
#include "aphid_migration_limited.h"

using namespace base;

namespace aphid {

PUBLISH(Aphid_migration_limited)
// Do not consider temperature yet
Aphid_migration_limited::Aphid_migration_limited(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(Wheat_Pheno);
    Input(Date);
    Input(Beginning);
    Input(End);
    Input(Daily_Nb_migrants); //daily immigration pressure: how many aphids landed in the field
    Input(AphidDensity);
    Input(DensityMax);
    Output(migration);
}

void Aphid_migration_limited::reset() {
    migration=
    AphidDensity=0;
}

void Aphid_migration_limited::update() {
    if (Wheat_Pheno>31 && Date>Beginning && Date<End && AphidDensity<DensityMax)
        //according to literature, the first migration wave does not start before GS 31
        //and also the earliest observation of aphids in the fields Mellomvoll, Cecile, Niels, Charles?
        //here to make it simple: migration goes from the 01/04 to the 30/06
       {migration=Daily_Nb_migrants;}
    else {migration=0;}
}


}
