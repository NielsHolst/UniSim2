#include <base/publish.h>
#include "aph_survivalD.h"

using namespace base;

namespace aphid {

PUBLISH(Aph_survivalD)

Aph_survivalD::Aph_survivalD(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (Wheat_Pheno);
    Input (Temperature);
    Output(survival);
}

void Aph_survivalD::reset() {
    survival=1;
}

void Aph_survivalD::update() {
    // From Duffy et al. 2017: same intrinsic mortality for each aphids
    if(Wheat_Pheno<=73)
    {survival=(94.4449-0.0000000332214*(exp(0.725604*Temperature)))/100;}
    if(Wheat_Pheno>73)
    {survival=0.45;}
    if(Wheat_Pheno>80)
    {survival=0;}
    // if Temperature >30, it may happen.
    if(survival<0)
    {survival=0;}
}


}
