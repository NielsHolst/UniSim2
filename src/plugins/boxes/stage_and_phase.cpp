#include <iostream>
#include <cmath>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include <base/vector_op.h>
#include "distributed_delay_2d.h"
#include "stage_and_phase.h"
	
using namespace base;

namespace boxes {

PUBLISH(StageAndPhase)

StageAndPhase::StageAndPhase(QString name, QObject *parent)
    : StageBase(name, parent)
{
    Input(phaseK).equals(30).help("Works like k but in the change phase process");
    Input(phaseDuration).equals(100.).help("Works like duration but in the change phase process");
    Input(inflow).help("Amount of inflow");
    Input(timeStep).equals(1).help("Time step for stage development");
    Input(phaseTimeStep).equals(1).help("Time step for phase development");
    Output(latestInflow).help("Amount that just flowed in");
    Output(outflowCohorts).help("Outflow cohorts (vector of length k) emerging from the stage");
    Output(outflowScalar).help("Outflow cohorts summed to a scalar, emerging from the stage");
}

void StageAndPhase::createDistributedDelay() {
    DistributedDelay2D::FixedParameters p;
    p.L1 = duration;
    p.L2 = phaseDuration;
    p.k1 = k;
    p.k2 = phaseK;
    p.minIter = 1;
    _ddBase = _dd = new DistributedDelay2D(p, this);
}

void StageAndPhase::reset() {
    StageBase::reset();
    if (inflow.isEmpty()) {
        inflow.resize(phaseK);
        latestInflow.resize(phaseK);
    }
    else {
        inflow.fill(0.);
        latestInflow.fill(0.);
    }
    outflowCohorts.resize(phaseK);

    if (phaseInflow.isEmpty())
        phaseInflow.resize(k);
    phaseOutflow.resize(k);

    inflowPending.resize(phaseK);
    phaseInflowPending.resize(k);
}

void StageAndPhase::update() {
    if (inflow.size() != phaseK) {
        QString msg("inflow's size (phaseK=%1) does not match the size of the recipient (phaseK=%2)");
        ThrowException(msg.arg(inflow.size()).arg(phaseK)).context( this);
    }
    if (phaseInflow.size() != k) {
        QString msg("phaseInflow's size (k=%1) does not match the size of the recipient (k=%2)");
        ThrowException(msg.arg(phaseInflow.size()).arg(k)).context( this);
    }

    applyInstantMortality();

    increment(inflowPending, inflow);
    latestInflow = inflow;
    inflowTotal += accum(inflow);

    increment(phaseInflowPending, phaseInflow);
    latestPhaseInflow = phaseInflow;
    phaseInflowTotal += accum(phaseInflow);

    // It remains to consider cases, when only one time step is zero
    if (TestNum::eqZero(timeStep) || TestNum::eqZero(phaseTimeStep)) {
        content = _dd->content() + accum(inflowPending) + accum(phaseInflowPending);
        outflowCohorts.fill(0.);
        outflowScalar = 0.;
        phaseOutflow.fill(0.);
        growth = 0.;
        return;
    }

    DistributedDelay2D::UpdateParameters p;
    p.dt1 = timeStep;
    p.dt2 = phaseTimeStep;
    p.inflow1 = &inflowPending;
    p.inflow2 = &phaseInflowPending;
    _dd->update(p);

    inflowPending.fill(0.);
    phaseInflowPending.fill(0.);

    content = _dd->content();
    outflowCohorts = _dd->state().outflow1;
    outflowScalar = accum(outflowCohorts);
    phaseOutflow = _dd->state().outflow2;

    outflowTotal += outflowScalar;
    phaseOutflowTotal += accum(phaseOutflow);
    growth = _dd->state().growthRate;
//    std::cout << qPrintable(id().label()) << " StageAndPhase::update() Z\n";
}

double StageAndPhase::growthDemand() {
    return 0;
}

} // namespace
