/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <boost/numeric/conversion/cast.hpp>
#include <QFile>
#include <QTextStream>
#include <base/convert.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>
#include <base/timer.h>
#include "simulation.h"

using namespace base;

namespace boxes {

PUBLISH(Simulation)

Simulation::Simulation(QString name, Box *parent)
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

void Simulation::initialize() {
    if (debug())
        dialog().information("Note: Debug is ON. Program will be slow");
    environment().isUnattended(unattended);
    iteration = 1;
    // Increment file counter;
    // output files needed for this run must be open at this time
    environment().incrementFileCounter();
}

void Simulation::run() {
    dialog().resetProgress();
    environment().isSilent(silent);
    if (silent)
        dialog().information("Running silently...");
    hasError = false;
    QElapsedTimer time;
    try {
        time.start();
        initializeFamily();
        for (iteration = 1;
             (useStopIterations && !stopIterations && iterations==1) ||         // apply flag only
             (useStopIterations && !stopIterations && iteration<=iterations) || // apply both lag and count
             (!useStopIterations && iteration<=iterations);                     // apply count only
              ++iteration)
        {
            resetFamily();
            for (step = 1;
                 (useStopSteps && !stopSteps && steps==1) ||    // apply flag only
                 (useStopSteps && !stopSteps && step<=steps) || // apply both flag and count
                 (!useStopSteps && step<=steps);                // apply count only
                 ++step)
            {
                show(time);
                updateFamily();
            }
            cleanupFamily();
        }
        debriefFamily();
    }
    catch (Exception &ex) {
        hasError = true;
        errorMsg = ex.what();
    }
    dialog().finishProgress();
    executionTime = boost::numeric_cast<int>(time.elapsed());
}

void Simulation::show(QElapsedTimer time) {
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
    if (unattended)
        dialog().information("The simulation has finished in unattended mode. Write 'clip' at the prompt before pasting the outputs to R.");
    dialog().finishProgress();
}


}
