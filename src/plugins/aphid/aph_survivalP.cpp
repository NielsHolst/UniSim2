#include <base/publish.h>
#include "aph_survivalP.h"

using namespace base;

namespace aphid {

PUBLISH(Aph_survivalP)

Aph_survivalP::Aph_survivalP(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (Wheat_Pheno);
    Output(survival);
}

void Aph_survivalP::reset() {
    survival=1;
}

void Aph_survivalP::update() {
    // From Plantegenest et al. 2001: same intrinsic mortality for each aphids
    if(Wheat_Pheno<=73)
    {survival=0.95;}
    if(Wheat_Pheno>73)
    {survival=0.4;}
}


}
