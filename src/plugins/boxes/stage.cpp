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
    Input(dt).imports(".../time[step]");
    Input(inflow);
    Input(initial);
    Input(phaseOutflowProportion); // Proportion that will change phase in next time step
    Output(latestInflow).trackOff();
    Output(outflow);
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
            throw Exception(msg.arg(phaseInflow.size()).arg(k), "", this);
        }
        double *contents = const_cast<double*>(data());
        increment(contents, phaseInflow.data(), k);
        phaseInflowTotal += accum(phaseInflow);
    }

    if (TestNum::eqZero(dt)) {
        sum = dd->content() + inflowPending;
        outflow = growth = 0.;
        return;
    }

    if (fgr <= 0)
        throw Exception(QString("Growth rate must be > 0"), QString::number(fgr), this);
    if (inflowPending < 0) {
        throw Exception(QString("Input must be >= 0"), QString::number(inflowPending), this);
    }

    dd->update(inflowPending, dt, fgr);
    inflowPending = 0;

    if (phaseOutflowProportion == 0.) {
        phaseOutflow.fill(0.);
    }
    else{
        TestNum::snapTo(phaseOutflowProportion, 1.);
        if (phaseOutflowProportion > 1.) {
            QString msg = "phaseOutflowProportion must not be > 1";
            throw Exception(msg, QString::number(phaseOutflowProportion), this);
        }
        phaseOutflow = dd->take(phaseOutflowProportion);
        phaseOutflowTotal += accum(phaseOutflow);
    }

    sum = dd->content();
    outflowTotal += outflow = dd->state().outflowRate;
    growth = dd->state().growthRate;
}

} // namespace
