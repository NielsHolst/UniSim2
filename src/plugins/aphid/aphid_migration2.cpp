#include <base/publish.h>
#include "aphid_migration2.h"

using namespace base;

namespace aphid {

PUBLISH(Aphid_migration2)
// Do not consider temperature yet
Aphid_migration2::Aphid_migration2(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(Wheat_Pheno);
    Input(Date);
    Input(perc_infest);
    Input (Daily_Nb_migrants); //daily immigration pressure: how many aphids landed in the field
    Output(migration);
    Output(migration_infect);
}

void Aphid_migration2::reset() {
    migration=
    migration_infect=0;
}

void Aphid_migration2::update() {
    if (Wheat_Pheno>31 && Date>90 && Date<181)
        //according to literature, the first migration wave does not start before GS 31
        //here to make it simple: migration goes from the 01/04 to the 30/06
       {migration=Daily_Nb_migrants;
        migration_infect=migration*perc_infest;}
    else {migration=
          migration_infect=0;}
}


}
