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

using namespace base;

namespace boxes {

PUBLISH(Simulation)

Simulation::Simulation(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(iterations).equals(1);
    Input(steps).equals(1);
    Input(stopIterations).equals(false);
    Input(stopSteps).equals(false);
    Input(useStopIterations).equals(false);
    Input(useStopSteps).equals(false);
    Output(iteration).noReset();
    Output(step);
    Output(finalStep);
    Output(executionTime);
    Output(hasError);
    Output(errorMsg);
}

Simulation::~Simulation() {
    if (environment().root() == this)
        environment().root(0);
}

void Simulation::amend() {
//    if (iterations > 1)
        port("iteration")->track();
}

void Simulation::run() {
    nextShowProgress = 0.01;
    hasError = false;
    QTime time;
    try {
        time.start();
        Track::effectuateOrders();
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
                 (useStopSteps && !stopSteps && step<=steps) || // apply both lag and count
                 (!useStopSteps && step<=steps);                // apply count only
                 ++step)
            {
                show(time);
                updateFamily();
                Track::updateAll();
            }
            environment().computationStep(ComputationStep::Cleanup);
            cleanupFamily();
            Track::cleanupAll();
        }
        environment().computationStep(ComputationStep::Debrief);
        debriefFamily();
    }
    catch (Exception &ex) {
        hasError = true;
        errorMsg = ex.what();
    }
    dialog().finished();
//    environment().computationStep(ComputationStep::Ready);
    executionTime = time.elapsed();
}

void Simulation::show(QTime time) {
    double progress = double(step)/steps/iterations;
    if (progress > nextShowProgress) {
        double total = time.elapsed()/progress;
        dialog().progress(convert<int>(time.elapsed())/1000, convert<int>(total)/1000);
        nextShowProgress += 0.01;
    }
}

void Simulation::cleanup() {
    finalStep = step;
    step = 0;
}

void Simulation::debrief() {
    environment().incrementFileCounter();
}


}
