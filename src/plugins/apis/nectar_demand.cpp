#include <base/any_year.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "nectar_demand.h"

using namespace base;

namespace apis {

PUBLISH(NectarDemand)

NectarDemand::NectarDemand(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes demand for nectar");
    Input(numberWorkers).help("Worker cohort numbers");
    Input(numberLarvae).help("Larva cohort numbers");
    Input(massIncrement).help("Larva cohort masses (mg)");
    Input(timeStepDays).imports("calendar[timeStepDays]");
    Input(conversionRatio).equals(1.5).help("Conversion ratio of honey needed to produce a worker bee (mg/mg)");
    Input(honeyConcentration).equals(0.18).help("Ratio of sugar/total mass");
    Input(nectarConcentration).imports("nectarFlow[concentration]");
    Input(workerDemand).equals(7).help("Honey used for worker bee respiration (mg/d/bee");
    Output(demandLarvae).help("Demand rate for larvae (g/d)");
    Output(demandWorkers).help("Demand rate for workers (g/d)");
    Output(demandTotal).help("Demand rate for workers + larvae (g/d)");
}

void NectarDemand::reset() {
}

void NectarDemand::update() {
    QVector<double> growth;
    growth.resize(numberLarvae.size());
    vector_op::multiply(growth, numberLarvae, massIncrement);

    // conversion from mg/time step to g/d and from nectar to honey concentration
    double conv = 1./1000./timeStepDays*honeyConcentration/nectarConcentration;

    demandLarvae = conv*vector_op::sum(growth)*conversionRatio;
    demandWorkers = conv*vector_op::sum(numberWorkers)*workerDemand;
    demandTotal = demandLarvae + demandWorkers;
}

}
