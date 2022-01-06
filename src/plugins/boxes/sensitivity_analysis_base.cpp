/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/dialog.h>
#include <base/distribution.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/port.h>
#include "sensitivity_analysis_base.h"

using namespace base;

namespace boxes {

SensitivityAnalysisBase::SensitivityAnalysisBase(QString name, QObject *parent)
    : Box(name, parent)
{
    setClassName("SensitivityAnalysisBase");
    help("is the sensitivity analysis base class");
    Input(sampleSize).equals(5).help("Size of sample in input parameter space (N)");
    Output(inputsAnalysed).noReset().help("Number of input ports included in analysis (k)");
    Output(inputsTotal).noReset().help("Total number of input ports in model");
    Output(iterations).noReset().help("Number of simulation iterations needed");
}

void SensitivityAnalysisBase::initialize() {
    setInputsTotal();
    setInputsAnalysed();
    reviewNumberOfSamples();
    iterations = numberOfIterations();
    for (Distribution *dist : _saDistributions)
        dist->initialize(numberOfSamples());
    QString msg = "Running sensitivity analysis for %1 iterations...";
    dialog().information(msg.arg(iterations));
}

void SensitivityAnalysisBase::setInputsTotal() {
    Box *root = findOne<Box*>("/");
    QVector<Port*> ports = root->findMany<Port*>("*[*]");
    int n = 0;
    for (Port *port : ports) {
        if (canBeAnalysed(port)) ++n;
    }
    inputsTotal = n;
}

bool SensitivityAnalysisBase::canBeAnalysed(Port *port) const {
    bool isInput = port->access()==PortAccess::Input,
        isInThisBox = port->boxParent()==this,
        isRoot = port->boxParent()==environment().root(),
        ok = isInput && !isInThisBox && !isRoot;
    return ok;
}

void SensitivityAnalysisBase::setInputsAnalysed() {
    _saDistributions.clear();
    Box *root = findOne<Box*>("/");
    QVector<Distribution*> distributions = root->findMany<Distribution*>("*<Distribution>");
    for (Distribution *dist : distributions) {
        if (!canBeAnalysed(dist->port())) {
            ThrowException("SensitivityAnalysis not allowed for this class")
                .value(dist->port()->boxParent()->fullName())
                .context(this);
        }
        _saDistributions << dist;
    }
    inputsAnalysed = _saDistributions.size();
    if (inputsAnalysed == 0)
        ThrowException("No ports set up for analysis (use @ notation)");
}

}
