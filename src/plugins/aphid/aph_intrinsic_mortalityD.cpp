#include <base/publish.h>
#include "aph_intrinsic_mortalityD.h"

using namespace base;

namespace aphid {

PUBLISH(Aph_intrinsic_mortalityD)

Aph_intrinsic_mortalityD::Aph_intrinsic_mortalityD(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (Wheat_Pheno);
    Input (Temperature);
    Output(intrinsic_mortality);
}

void Aph_intrinsic_mortalityD::reset() {
    intrinsic_mortality=0;
}

void Aph_intrinsic_mortalityD::update() {
    // From Duffy et al. 2017: same intrinsic mortality for each aphids
    if(Wheat_Pheno<=73)
    {intrinsic_mortality=(100-(94.4449-0.0000000332214*(exp(0.725604*Temperature))))/100;}
    if(Wheat_Pheno>73)
    {intrinsic_mortality=0.55;}
    // if Temperature >30, it may happen.
    if(intrinsic_mortality>1)
    {intrinsic_mortality=1;}
}


}
