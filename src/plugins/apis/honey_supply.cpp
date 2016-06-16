#include <base/publish.h>
#include <base/test_num.h>
#include <base/vector_op.h>
#include "honey_supply.h"

using namespace base;
using vector_op::sum;

namespace apis {

PUBLISH(HoneySupply)

HoneySupply::HoneySupply(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes potential and realised honey supply");
    Input(cohortNumbers).help("Forager cohort numbers");
    Input(cropVolume).equals(40).help("Volume of forager crop (ml)");
    Input(handlingTime).equals(10).help("Interval between foraging trips (min)");
    Input(apparency).equals(1).help("Apparency of ressource ]0;1]");
    Input(conversionFactor).equals(1).help("Convertion factor from nectar to honey (mg/ml)");
    Input(nectarFlow).imports("nectarFlow[flow]");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Output(value).help("Potential supply of honey over this time step (g)");
}

void HoneySupply::update() {
    double demandRate = sum(cohortNumbers)*cropVolume/handlingTime,
           supplyRate = TestNum::eqZero(demandRate) ? 0 : demandRate*(1. - exp(-apparency*nectarFlow/demandRate));
    value = conversionFactor*supplyRate*timeStepSecs*60;
}

}
