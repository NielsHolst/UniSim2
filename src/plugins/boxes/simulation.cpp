#include <base/convert.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/path.h>
#include <base/port.h>
#include <base/port_type.h>
#include <base/publish.h>
#include <base/timer.h>
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
    Output(iteration).noReset();
    Output(step);
    Output(finalStep);
    Output(executionTime);
    Output(hasError);
    Output(errorMsg);
}

void Simulation::amend() {
    if (iterations > 1)
        port("iteration")->track();
}

void Simulation::run() {
    dialog().message("Running...");
    nextShowProgress = 0.01;
    hasError = false;
    QTime time;
    try {
        time.start();
        dialog().information("initialize...");
        initializeFamily();
        for (iteration = 1; !stopIterations && iteration <= iterations; ++iteration) {
            dialog().information("reset...");
            resetFamily();
            dialog().information("update...");
            for (step = 1; !stopSteps && step <= steps; ++step) {
                show(time);
                updateFamily();
            }
            dialog().information("cleanup...");
            cleanupFamily();
        }
        dialog().information("debrief...");
        debriefFamily();
    }
    catch (Exception &ex) {
        hasError = true;
        errorMsg = ex.what();
    }
    dialog().finished();
    dialog().message("Ready");
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
