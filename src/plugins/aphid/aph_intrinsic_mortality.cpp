#include <base/publish.h>
#include "aph_intrinsic_mortality.h"

using namespace base;

namespace aphid {

PUBLISH(Aph_intrinsic_mortality)

Aph_intrinsic_mortality::Aph_intrinsic_mortality(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (Wheat_Pheno);
    Output(intrinsic_mortality);
}

void Aph_intrinsic_mortality::reset() {
    intrinsic_mortality=0;
}

void Aph_intrinsic_mortality::update() {
    // From Plantegenest et al. 2001: same intrinsic mortality for each aphids
    if(Wheat_Pheno<=73)
    {intrinsic_mortality=(-1/4) * log(0.95);}
    if(Wheat_Pheno>73)
    {intrinsic_mortality=(-1/4) * log(0.4);}
}


}
