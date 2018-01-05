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

//#include <QMessageBox>
using namespace base;

namespace boxes {

PUBLISH(Stage)

Stage::Stage(QString name, QObject *parent)
    : StageBase(name, parent)
{
    Class(Stage);
    help("delays inflow to emerge as a time-distributed outflow");
    Input(inflow).help("Amount of inflow");
    Input(phaseOutflowProportion).help("Proportion that will change phase in next time step");
    Output(outflow).help("Outflow emerging from the stage");
}

void Stage::createDistributedDelay() {
    DistributedDelay::Parameters p;
    p.L = duration;
    p.k = k;
    p.minIter = 1;
    _ddBase = _dd = new DistributedDelay(p, this);
}

void Stage::myReset() {
    content = initial;
    outflow = 0.;
    _firstUpdate = true;
    update();   // Otherwise, outflow will be one simulation step late
}

void Stage::update() {
    if (content < zeroLimit)
        applyInstantMortality(1.);
    else
        applyInstantMortality(instantLossRate);

    inflowSum = inflow;
    inflowTotal += inflow;
    if (_firstUpdate)
        inflow += initial;

    if (!phaseInflow.isEmpty()) {
        if (phaseInflow.size() != k) {
            QString msg("phaseInflow's size (k=%1) does not match the size of the recipient (k=%2)");
            ThrowException(msg.arg(phaseInflow.size()).arg(k)).context(this);
        }
        double *contents = const_cast<double*>(data());
        increment(contents, phaseInflow.data(), k);
        phaseInflowSum = accum(phaseInflow);
        phaseInflowTotal += phaseInflowSum;
    }

    // Replace zero time step with neglible time step
    if (timeStep == 0)
        timeStep = 1e-12;

    // Replace zero growth with neglible growth
    if (growthFactor == 0)
        growthFactor = 1e-12;
    else if (growthFactor < 0)
        ThrowException("Growth rate must be > 0").value(growthFactor).context(this);
    if (inflow < 0)
        ThrowException("Inflow must be >= 0").value(inflow).context(this);

    _dd->update(inflow, timeStep, growthFactor);

    if (phaseOutflowProportion == 0.) {
        phaseOutflow.fill(0.);
    }
    else{
        TestNum::snapTo(phaseOutflowProportion, 1.);
        if (phaseOutflowProportion > 1.) {
            QString msg = "phaseOutflowProportion must not be > 1";
            ThrowException(msg).value(phaseOutflowProportion).context(this);
        }
        phaseOutflow = _dd->take(phaseOutflowProportion);
        phaseOutflowSum = accum(phaseOutflow);
        phaseOutflowTotal += accum(phaseOutflow);
    }

    content = _dd->content();
    outflow = _dd->state().outflowRate;
    outflowSum = outflow;
    outflowTotal += outflow;
    growth = _dd->state().growthRate;
    if (_firstUpdate) {
        inflow -= initial;
        _firstUpdate = false;
    }
}

} // namespace
