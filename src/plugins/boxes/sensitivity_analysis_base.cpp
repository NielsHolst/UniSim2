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
    Class(SensitivityAnalysisBase);
    help("sensitivity analysis base class");
    Input(method).equals("LHS").help("For sampling use either Latin hypercube sampling ('LHS') or Monte Carlo ('MC')");
    Input(sampleSize).equals(30).help("Size of sample in input parameter space (N)");
    Output(inputsAnalysed).help("Total number of input ports (k)");
    Output(inputsTotal).help("Number of input ports included in analysis");
    Output(iterations).noReset().help("Number of simulation iterations needed");
}

void SensitivityAnalysisBase::initialize() {
    setMethod();
    setInputsTotal();
    setInputsAnalysed();
    iterations = numberOfIterations();
    for (Distribution *dist : _saDistributions)
        dist->initialize(numberOfSamples());
}

void SensitivityAnalysisBase::setMethod() {
    if (method=="MC")
        _method = MC;
    else if (method=="LHS")
        _method = LHS;
    else
        ThrowException("Sampling method must be one 'MC' or 'LHS'")
                .value(method).context(this);
}

void SensitivityAnalysisBase::setInputsTotal() {
    Box *root = findOne<Box>("/");
    QVector<Port*> ports = root->findMany<Port>("*[*]");
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
    Box *root = findOne<Box>("/");
    QVector<Distribution*> distributions = root->findMany<Distribution>("*<Distribution>");
    for (Distribution *dist : distributions) {
        if (!canBeAnalysed(dist->port())) {
            ThrowException("SensitivityAnalysis not allowed for this class")
                .value(dist->port()->boxParent()->fullName())
                .context(this);
        }
        _saDistributions << dist;
    }
    inputsAnalysed = _saDistributions.size();
}

void SensitivityAnalysisBase::reset() {
    for (Distribution *dist : _saDistributions) {
        double value = dist->draw();
        dist->port()->equals(value);
    }
}

}
