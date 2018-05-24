#include <base/publish.h>
#include "wheat_growth.h"

using namespace base;

namespace aphid {

PUBLISH(Wheat_growth)

Wheat_growth::Wheat_growth(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(Degree_days);
    Output(Pheno_stage);
}

void Wheat_growth::reset() {
    theta1=-1.9427;
    theta2=-0.0031;
    theta3=95.2895;
    Pheno_stage=0;
}

void Wheat_growth::update() {
        Pheno_stage=theta3*exp(theta1*exp(theta2*Degree_days));
}

}
