#include <base/distribution.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/port.h>
#include <base/publish.h>
#include "sensitivity_analysis.h"

using namespace base;

namespace boxes {

PUBLISH(SensitivityAnalysis)

SensitivityAnalysis::SensitivityAnalysis(QString name, QObject *parent)
    : Box(name, parent)
{
    help("runs a sensitivity analysis");
    Input(iterations).imports("ancestors::*[iterations]");
    Input(method).help("Use either Latin hypercube sampling ('LHS') or Monte Carlo ('MC')").equals("LHS");
    Output(inputsAnalysed).help("Total number of input ports");
    Output(inputsTotal).help("Number of input ports included in analysis");
}

void SensitivityAnalysis::initialize() {
    setMethod();
    setInputsTotal();
    setInputsAnalysed();
    int numStrata = (_method == MC) ? 1 : iterations;
    for (Distribution *dist : _saDistributions)
        dist->initialize(numStrata);
}

void SensitivityAnalysis::setMethod() {
    if (method=="MC")
        _method = MC;
    else if (method=="LHS")
        _method = LHS;
    else
        ThrowException("SA method must be one 'MC' or 'LHS'")
                .value(method).context(this);
}

void SensitivityAnalysis::setInputsTotal() {
    Box *root = findOne<Box>("/");
    QVector<Port*> ports = root->findMany<Port>("*[*]");
    int n = 0;
    for (Port *port : ports) {
        if (canBeAnalysed(port)) ++n;
    }
    inputsTotal = n;
}

bool SensitivityAnalysis::canBeAnalysed(Port *port) const {
    bool isInput = port->access()==PortAccess::Input,
        isInThisBox = port->boxParent()==this,
        isRoot = port->boxParent()==environment().root(),
        ok = isInput && !isInThisBox && !isRoot;
    return ok;
}

void SensitivityAnalysis::setInputsAnalysed() {
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

void SensitivityAnalysis::reset() {
    for (Distribution *dist : _saDistributions) {
        double value = dist->draw();
        dist->port()->equals(value);
    }
}

}
