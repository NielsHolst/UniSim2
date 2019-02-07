#include <base/publish.h>
#include "end_migration.h"

using namespace base;

namespace aphid {

PUBLISH(End_migration)
// Do not consider temperature yet
End_migration::End_migration(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(GS);
    Input(GS_max);
    Input(Date);
    Output(stopping_date);
}

void End_migration::reset() {
    stopping_date=250;
}

void End_migration::update() {

    if(GS>GS_max)
    {stopping_date=Date;}
}
}
