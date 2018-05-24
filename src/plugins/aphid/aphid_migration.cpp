#include <base/publish.h>
#include "aphid_migration.h"

using namespace base;

namespace aphid {

PUBLISH(Aphid_migration)
// Do not consider temperature yet
Aphid_migration::Aphid_migration(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(Wheat_Pheno);
    Input(Date);
    Input (Daily_Nb_migrants); //daily immigration pressure: how many aphids landed in the field
    Output(migration);
}

void Aphid_migration::reset() {
    migration=0;
}

void Aphid_migration::update() {
    if (Wheat_Pheno>31 && Date>90 && Date<181)
        //according to literature, the first migration wave does not start before GS 31
        //here to make it simple: migration goes from the 01/04 to the 30/06
       {migration=Daily_Nb_migrants;}
    else {migration=0;}
}


}
