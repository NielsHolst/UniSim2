#include <base/publish.h>
#include "wheat_growth_AG.h"

using namespace base;

namespace aphid {

PUBLISH(Wheat_growth_AG)

Wheat_growth_AG::Wheat_growth_AG(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(Degree_days);
    Input(Min);
    Input(ED50);
    Output(Pheno_stage);
}

void Wheat_growth_AG::reset() {
    upper_limit=90; // maximum value of the phenological stage
    slope=-4.2; // slope of the logistic function
    Pheno_stage=0;
}

void Wheat_growth_AG::update() {
    Pheno_stage= (Degree_days>0) ? Min+ ( (upper_limit-Min)/(1+exp(slope* (log(Degree_days)-log(ED50) ) ) )) : 0.;
}

}
