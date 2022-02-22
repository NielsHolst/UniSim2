/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/time_unit.h>
#include <base/publish.h>
#include "simulation_period.h"

using namespace base;

namespace boxes {

PUBLISH(SimulationPeriod)

SimulationPeriod::SimulationPeriod(QString name, Box *parent)
    : Box(name, parent)
{
    help("defines a time interval for the simulation");
    Input(begin).help("Simulation start").unit("DateTime");
    Input(end).help("Simulation end").unit("DateTime");
    Input(timeStep).equals(2).help("Time step length").unit("-");
    Input(timeUnit).equals("m").help("Time step unit").unit("y|d|h|m|s");
    Output(steps).help("Number of steps in simulation").unit("-");
}

void SimulationPeriod::initialize() {
    if (begin > end) {
        ThrowException("Wrong time interval: begin must be before end").
                value(begin).value2(end).context(this);
    }
}

void SimulationPeriod::reset() {
    // If end is at midnight assume that the whole date is included
    auto totalSecs = begin.secsTo(end);
    if (end.time() == QTime(0,0,0))
        totalSecs += 24*60*60;

    // Compute number of steps in simulation
    auto stepSecs  = timeStep*Time::toSeconds(timeUnit);
    steps = totalSecs/stepSecs;
    if (totalSecs%stepSecs > 0)
        ++steps;
}

} //namespace

