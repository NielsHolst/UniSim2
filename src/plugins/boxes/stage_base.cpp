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
    : Box(name, parent), ddBase(0)
{
    Input(k).equals(30);
    Input(duration).equals(100);
    Input(fgr).equals(1);
    Input(sdRatio).equals(1);
    Input(instantMortality).equals(0);  // Mortality [0..100] will be applied in the next time step, before @F inflow is added
    Input(instantLossRate).equals(0);   // Works just like @F mortality except the scale is a ratio [0..1]
    Input(phaseInflow);                 // Inflow of dimension @F {k}
    Output(sum);
    Output(inflowTotal).trackOff();
    Output(phaseInflowTotal).trackOff();
    Output(phaseOutflowTotal).trackOff();
    Output(growth).trackOff();
    Output(phaseOutflow).trackOff();
}

StageBase::~StageBase() {
    delete ddBase;
}

void StageBase::initialize()
{
    if (k <= 0)
        throw Exception(QString("k must be > 0"), QString::number(k), this);

    if (duration <= 0)
        throw Exception(QString("Duration must be > 0"), QString::number(duration), this);

    ddBase = createDistributedDelay();
}


void StageBase::reset()
{
    Q_ASSERT(ddBase);
    ddBase->scale(0);
    sum = inflowTotal = outflowTotal = phaseInflowTotal = phaseOutflowTotal = growth = 0.;
    phaseOutflow.resize(k);
}

void StageBase::applyInstantMortality() {
    if (instantMortality > 0. && instantLossRate > 0.) {
        QString msg = "Parameters instantMortality and instantLossRate cannot both be > 0 (they are '%1' and '%2')";
        throw Exception(msg.arg(instantMortality).arg(instantLossRate));
    }
    double survival = 1. - instantMortality/100. - instantLossRate;
    TestNum::snapToZero(survival);
    TestNum::snapTo(survival, 1.);
    if (survival < 0 || survival > 1.)
       throw Exception(QString("Survival must be in the range [0;1]"), QString::number(survival), this);
    ddBase->scale(survival);
    instantMortality = instantLossRate = 0;
}


const double* StageBase::data() {
    return ddBase->data();
}

} // namespace
