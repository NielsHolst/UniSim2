/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/port.h>
#include "energy_budget_optimiser.h"

using namespace base;

namespace vg {

PUBLISH(EnergyBudgetOptimiser)

EnergyBudgetOptimiser::EnergyBudgetOptimiser(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes average of inputs");
    Input(inputs).help("Input values");
    Output(value).help("Average of inputs");
}

void EnergyBudgetOptimiser::initialize() {
    ports.clear();
    for (QString input : inputs)
        ports << findMany<Port>(input);
}

void EnergyBudgetOptimiser::update() {
    if (ports.isEmpty())
        return;
    value = 0;
    for (Port *port : ports)
        value += port->value<double>();
    value /= ports.size();
}

} //namespace

