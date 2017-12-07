/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <cmath>
#include <QString>
#include <QTextStream>
#include <base/test_num.h>
#include <base/exception.h>
#include <base/test_num.h>
#include "stage_base.h"

using namespace base;

namespace boxes {

StageBase::StageBase(QString name, QObject *parent)
    : Box(name, parent), _ddBase(0)
{
    Input(initial).help("Initial amount of inflow");
    Input(k).equals(30).help("Distribution parameter; use small k for larger dispersion in output");
    Input(duration).equals(100).help("Average delay between inflow and outflow");
    Input(growthFactor).equals(1).help("Factor by which outflow will be scaled relative to inflow");
    Input(instantLossRate).help("Mortality applied before inflow is added [0..1]");
    Input(phaseInflow).help("Phase inflow cohorts (vector)");

    Output(content).help("Total content inside the stage");
    Output(inflowTotal).help("Accumulated total stage inflow (scalar)");
    Output(outflowTotal).help("Accumulated total stage outflow (scalar)");
    Output(phaseInflowTotal).help("Phase inflow cohorts total (scalar)");
    Output(phaseOutflowTotal).help("Phase outflow cohorts total (scalar)");
    Output(phaseOutflow).help("Phase outflow cohorts (vector)");
    Output(growth).help("Change in content+outflow during this time step");
}

StageBase::~StageBase() {
    delete _ddBase;
}

void StageBase::reset()
{
    if (k <= 0)
        ThrowException("k must be > 0").value(k).context(this);
    if (duration <= 0)
        ThrowException("Duration must be > 0").value(duration).context(this);
    delete _ddBase;
    createDistributedDelay();
    content = inflowTotal = outflowTotal = phaseInflowTotal = phaseOutflowTotal = growth = 0.;
    phaseOutflow.resize(k);
}

void StageBase::applyInstantMortality() {
    double survival = 1. - instantLossRate;
    TestNum::snapToZero(survival);
    TestNum::snapTo(survival, 1.);
    if (survival < 0 || survival > 1.)
       ThrowException("Survival must be in the range [0;1]").value(survival).context(this);
    _ddBase->scale(survival);
}


const double* StageBase::data() {
    return _ddBase->data();
}

} // namespace
