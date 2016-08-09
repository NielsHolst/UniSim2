/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <cmath>
#include <QString>
#include <QTextStream>
#include <base/box.h>
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "distributed_delay.h"
#include "stage.h"

using namespace base;

namespace boxes {

PUBLISH(Stage)

Stage::Stage(QString name, QObject *parent)
    : StageBase(name, parent), dd(0)
{
    help("delays inflow to emerge as a time-distributed outflow");
    Input(dt).equals(1).help("Time step");
    Input(inflow).help("Amount of inflow");
    Input(initial).help("Initial amount of inflow");
    Input(phaseOutflowProportion).help("Proportion that will change phase in next time step");
    Output(latestInflow).help("Amount that just flowed in");
    Output(outflow).help("Outflow emerging from the stage");
}

DistributedDelayBase* Stage::createDistributedDelay() {
    DistributedDelay::Parameters p;
    p.L = duration;
    p.k = k;
    p.minIter = 1;
    delete dd;
    return dd = new DistributedDelay(p, this);
}

void Stage::reset() {
    StageBase::reset();
    inflowPending = latestInflow = outflow = 0.;
    content = initial;
    firstUpdate = true;
}

void Stage::update() {
    applyInstantMortality();

    latestInflow = 0.;
    if (firstUpdate) {
        inflowPending += initial;
        inflowTotal += initial;
        latestInflow += initial;
        firstUpdate = false;
    }

    inflowPending += inflow;
    inflowTotal += inflow;
    latestInflow += inflow;

    if (!phaseInflow.isEmpty()) {
        if (phaseInflow.size() != k) {
            QString msg("phaseInflow's size (k=%1) does not match the size of the recipient (k=%2)");
            ThrowException(msg.arg(phaseInflow.size()).arg(k)).context(this);
        }
        double *contents = const_cast<double*>(data());
        increment(contents, phaseInflow.data(), k);
        phaseInflowTotal += accum(phaseInflow);
    }

    if (TestNum::eqZero(dt)) {
        content = dd->content() + inflowPending;
        outflow = growth = 0.;
        return;
    }

    if (growthFactor <= 0)
        ThrowException("Growth rate must be > 0").value(growthFactor).context(this);
    if (inflowPending < 0) {
        ThrowException("Input must be >= 0").value(inflowPending).context(this);
    }

    dd->update(inflowPending, dt, growthFactor);
    inflowPending = 0;

    if (phaseOutflowProportion == 0.) {
        phaseOutflow.fill(0.);
    }
    else{
        TestNum::snapTo(phaseOutflowProportion, 1.);
        if (phaseOutflowProportion > 1.) {
            QString msg = "phaseOutflowProportion must not be > 1";
            ThrowException(msg).value(phaseOutflowProportion).context(this);
        }
        phaseOutflow = dd->take(phaseOutflowProportion);
        phaseOutflowTotal += accum(phaseOutflow);
    }

    content = dd->content();
    outflowTotal += outflow = dd->state().outflowRate;
    growth = dd->state().growthRate;
}

} // namespace
