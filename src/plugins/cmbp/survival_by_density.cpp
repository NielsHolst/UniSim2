#include <base/publish.h>
#include "survival_by_density.h"

using namespace base;

namespace cmbp {

PUBLISH(SurvivalByDensity)

SurvivalByDensity::SurvivalByDensity(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(e1).help("Regression parameter");
    Input(N).help("Adult density");
    Output(value).help("Peak curve");
}

void SurvivalByDensity::reset() {
    update();
}

void SurvivalByDensity::update() {
    value = exp(e1*sqrt(N));
}


}
