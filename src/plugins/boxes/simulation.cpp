/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/convert.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/path.h>
#include <base/port.h>
#include <base/port_type.h>
#include <base/publish.h>
#include <base/timer.h>
#include <base/track.h>
#include "simulation.h"

#include <QFile>
#include <QTextStream>

using namespace base;

namespace boxes {

PUBLISH(Simulation)

Simulation::Simulation(QString name, QObject *parent)
    : Box(name, parent)
{
    help("runs a simulation");
    Input(iterations).equals(1).help("Number of iterations to run");
    Input(steps).equals(1).help("Number of steps in one iteration");
    Input(stopIterations).equals(false).help("Stop running after this iteration?");
    Input(stopSteps).equals(false).help("Stop running after this step?");
    Input(useStopIterations).equals(false).help("Use the stopIterations flag?");
    Input(useStopSteps).equals(false).help("Use the stopSteps flag?");
    Input(silent).equals(false).help("Run without scrolling progress messages?");
    Input(unattended).equals(false).help("An unattended run will finish without accessing clipboard");
    Output(iteration).noReset().help("Iteration number (1,2,...)");
    Output(step).help("Step number (0,1,2,...)");
    Output(finalStep).help("Last completed step in simulation");
    Output(executionTime).help("Duration of simulation run (ms)");
    Output(hasError).help("Did an error occur during simulation run?");
    Output(errorMsg).help("Error message");
}

Simulation::~Simulation() {
    if (environment().root() == this)
        environment().root(nullptr);
}

void Simulation::amend() {
    port("iteration")->track();
    Track::effectuateOrders();
}

void Simulation::initialize() {
    environment().isUnattended(unattended);
    iteration = 1;
}

void Simulation::run() {
    dialog().resetProgress();
    environment().isSilent(silent);
    if (silent)
        dialog().information("Running silently...");
    hasError = false;
    QTime time;
    try {
        time.start();
//        Track::effectuateOrders();
        environment().computationStep(ComputationStep::Initialize);
        initializeFamily();

        for (iteration = 1;
             (useStopIterations && !stopIterations && iterations==1) ||         // apply flag only
             (useStopIterations && !stopIterations && iteration<=iterations) || // apply both lag and count
             (!useStopIterations && iteration<=iterations);                     // apply count only
              ++iteration)
        {
            environment().computationStep(ComputationStep::Reset);
            resetFamily();
            Track::resetAll();
            environment().computationStep(ComputationStep::Update);

            for (step = 1;
                 (useStopSteps && !stopSteps && steps==1) ||    // apply flag only
                 (useStopSteps && !stopSteps && step<=steps) || // apply both flag and count
                 (!useStopSteps && step<=steps);                // apply count only
                 ++step)
            {
                show(time);
                updateFamily();
//                Track::updateAll();
            }
            environment().computationStep(ComputationStep::Cleanup);
            cleanupFamily();
//            Track::cleanupAll();
        }
        environment().computationStep(ComputationStep::Debrief);
        debriefFamily();
    }
    catch (Exception &ex) {
        hasError = true;
        errorMsg = ex.what();
    }
    // environment().computationStep(ComputationStep::Ready);
    dialog().finishProgress();
    executionTime = time.elapsed();
}

void Simulation::show(QTime time) {
    DialogBase::ProgressInfo info;
    info.time = time;
    info.step = step;
    info.steps = steps;
    info.iteration = iteration;
    info.iterations = iterations;
    dialog().updateProgress(info);
}

void Simulation::cleanup() {
    finalStep = step;
    step = 0;
}

void Simulation::debrief() {
    environment().incrementFileCounter();
    dialog().finishProgress();
}


}
