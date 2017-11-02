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
    Input(timeStep).equals(1).help("Time step");
    Input(inflow).help("Amount of inflow");
    Input(initial).help("Initial amount of inflow");
    Input(phaseOutflowProportion).help("Proportion that will change phase in next time step");
    Output(latestInflow).help("Amount that just flowed in");
    Output(outflow).help("Outflow emerging from the stage");
}

void Stage::createDistributedDelay() {
    DistributedDelay::Parameters p;
    p.L = duration;
    p.k = k;
    p.minIter = 1;
    _ddBase = _dd = new DistributedDelay(p, this);
}

void Stage::reset() {
    StageBase::reset();
    inflowPending = latestInflow = outflow = 0.;
    content = initial;
    firstUpdate = true;
    update();   // Otherwise, outflow will be one simulation step late
}

void Stage::update() {
//    QMessageBox::information(0, "A Stage::update()", QString::number(instantLossRate) + " " + QString::number(content) + " " + QString::number(_dd->content()));
    applyInstantMortality();
//    QMessageBox::information(0, "B Stage::update()", QString::number(instantLossRate) + " " + QString::number(content) + " " + QString::number(_dd->content()));

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

//    if (TestNum::eqZero(timeStep)) {
//        content = _dd->content() + inflowPending;
//        outflow = growth = 0.;
//        return;
//    }

    if (growthFactor <= 0)
        ThrowException("Growth rate must be > 0").value(growthFactor).context(this);
    if (inflowPending < 0) {
        ThrowException("Input must be >= 0").value(inflowPending).context(this);
    }

//    QMessageBox::information(0, "C Stage::update()", QString::number(instantLossRate) + " " + QString::number(content) + " " + QString::number(_dd->content()));
    _dd->update(inflowPending, timeStep, growthFactor);
//    QMessageBox::information(0, "D Stage::update()", QString::number(instantLossRate) + " " + QString::number(content) + " " + QString::number(_dd->content()));
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
        phaseOutflow = _dd->take(phaseOutflowProportion);
        phaseOutflowTotal += accum(phaseOutflow);
    }

    content = _dd->content();
    outflowTotal += outflow = _dd->state().outflowRate;
    growth = _dd->state().growthRate;
//    if (instantLossRate>0.01) {
//        QMessageBox::information(0, "D", QString::number(instantLossRate) + " " + QString::number(dd->content()));
//    }
}

} // namespace
