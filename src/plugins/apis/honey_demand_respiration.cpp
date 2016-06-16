#include <base/publish.h>
#include <base/vector_op.h>
#include "honey_demand_respiration.h"

using namespace base;
using vector_op::sum;

namespace apis {

PUBLISH(HoneyDemandRespiration)

HoneyDemandRespiration::HoneyDemandRespiration(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes honey demand for adult respiration");
    Input(cohortNumbers).help("Worker cohort numbers");
    Input(respirationRate).equals(7).help("Honey used for worker bee respiration (mg/d/bee");
    Input(timeStepDays).imports("calendar[timeStepDays]");
    Output(value).help("Respiration demand over this time step (g)");
}

void HoneyDemandRespiration::update() {
    value = sum(cohortNumbers)*respirationRate*timeStepDays/1000.;
}

}
