#include <base/publish.h>
#include <base/vector_op.h>
#include "honey_demand_growth.h"

using namespace base;
using namespace vector_op;

namespace apis {

PUBLISH(HoneyDemandGrowth)

HoneyDemandGrowth::HoneyDemandGrowth(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes honey demand for juvenile growth");
    Input(cohortNumbers).help("Larva cohort numbers");
    Input(cohortDemands).help("Larva cohort growth demands (mg)");
    Input(cost).equals(0.3).help("Larva cohort numbers");
    Output(value).help("Growth demand over this time step, including conversion cost (g)");
}

void HoneyDemandGrowth::reset() {
}

void HoneyDemandGrowth::update() {
    QVector<double> cohortDemandsTotal;
    product(cohortDemandsTotal,
            cohortNumbers, cohortDemands);

    value = sum(cohortDemandsTotal)/(1-cost)/1000.;
}

}
