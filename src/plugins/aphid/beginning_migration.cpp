#include <base/publish.h>
#include "beginning_migration.h"

using namespace base;

namespace aphid {

PUBLISH(Beginning_migration)
// Do not consider temperature yet
Beginning_migration::Beginning_migration(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(Temperature);
    Input(Date);
    Output(starting_date);
}

void Beginning_migration::reset() {
    AprilDD=
    MayDD=0;
    starting_date=120;
}

void Beginning_migration::update() {
    if (Date>90 && Date<120 && Temperature>0)
    {AprilDD=AprilDD+Temperature;}
    if (Date>120 && Date<151 && Temperature>0)
    {MayDD=MayDD+Temperature;}
    if(Date>151)
    {starting_date=265-10.2*log(AprilDD)-31.1*log(MayDD);} // Hansen 2006 migration model in Scandinavia
}
}
