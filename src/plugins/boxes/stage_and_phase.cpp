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
    : StageBase(name, parent), _firstUpdate(true)
{
    Input(inflow).help("Amount of inflow (vector)");
    Input(timeStep).equals(1).help("Time step for stage development");
    Input(phaseK).equals(30).help("Phase distribution parameter");
    Input(phaseDuration).equals(100.).help("Average delay between inflow and outflow through phase");
    Input(phaseTimeStep).equals(1).help("Time step for phase development");

    Output(latestInflow).help("Amount that just flowed in (vector)");
    Output(stageOutflow).help("Stage outflow cohorts (vector)");
    Output(stageOutflowSum).help("Stage outflow cohorts summed (scalar)");
    Output(phaseOutflowSum).help("Phase outflow cohorts summed (scalar)");
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
    stageOutflow.resize(phaseK);

    if (phaseInflow.isEmpty())
        phaseInflow.resize(k);
    phaseOutflow.resize(k);

    _inflowPending.resize(phaseK);
    _phaseInflowPending.resize(k);
    update();   // Otherwise, outflow will be one simulation step late
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

    if (_firstUpdate) {
        _inflowPending[0] += initial;
        inflowTotal += initial;
        latestInflow[0] += initial;
        _firstUpdate = false;
    }
    increment(_inflowPending, inflow);
    latestInflow = inflow;
    inflowTotal += accum(inflow);

    increment(_phaseInflowPending, phaseInflow);
    latestPhaseInflow = phaseInflow;
    phaseInflowTotal += accum(phaseInflow);

    // Replace zero time step with neglible time step
    if (timeStep == 0)
        timeStep = 1e-12;
    if (phaseTimeStep == 0)
        phaseTimeStep = 1e-12;

//    // It remains to consider cases, when only one time step is zero
//    if (TestNum::eqZero(timeStep) || TestNum::eqZero(phaseTimeStep)) {
//        content = _dd->content() + accum(_inflowPending) + accum(_phaseInflowPending);
//        stageOutflow.fill(0.);
//        stageOutflowSum = 0.;
//        phaseOutflowSum = 0.;
//        phaseOutflow.fill(0.);
//        growth = 0.;
//        return;
//    }

    DistributedDelay2D::UpdateParameters p;
    p.dt1 = timeStep;
    p.dt2 = phaseTimeStep;
    p.inflow1 = &_inflowPending;
    p.inflow2 = &_phaseInflowPending;
    p.fgr1 = growthFactor;
    p.fgr2 = 1.;
    _dd->update(p);

    _inflowPending.fill(0.);
    _phaseInflowPending.fill(0.);

    content = _dd->content();
    stageOutflow = _dd->state().outflow1;
    stageOutflowSum = accum(stageOutflow);
    phaseOutflow = _dd->state().outflow2;
    phaseOutflowSum = accum(phaseOutflow);

    outflowTotal += stageOutflowSum;
    phaseOutflowTotal += phaseOutflowSum;
    growth = _dd->state().growthRate;
}

} // namespace
