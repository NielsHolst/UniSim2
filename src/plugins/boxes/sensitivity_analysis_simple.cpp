/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/distribution.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/port.h>
#include <base/publish.h>
#include "sensitivity_analysis_simple.h"

using namespace base;

namespace boxes {

PUBLISH(SensitivityAnalysisSimple)

SensitivityAnalysisSimple::SensitivityAnalysisSimple(QString name, QObject *parent)
    : SensitivityAnalysisBase(name, parent)
{
    help("runs a simple sensitivity analysis");
    Input(method).help("Not used");
    Input(simIterations).imports("/*[iterations]");
}

void SensitivityAnalysisSimple::reset() {
    // Draw new values at every reset
    for (Distribution *dist : _saDistributions) {
        double value = dist->draw();
        dist->port()->equals(value);
    }
}

int SensitivityAnalysisSimple::numberOfIterations() const {
    return simIterations;
}

int SensitivityAnalysisSimple::numberOfSamples() const {
    return simIterations;
}

}
