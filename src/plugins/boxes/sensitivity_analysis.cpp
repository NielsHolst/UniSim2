#include <base/distribution.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "sensitivity_analysis.h"

using namespace base;

namespace boxes {

PUBLISH(SensitivityAnalysis)

SensitivityAnalysis::SensitivityAnalysis(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(iterations).equals(1);
    Input(method).equals("MC");
    Output(inputsAnalysed);
    Output(inputsTotal);
}

void SensitivityAnalysis::amend() {
    Box *root = findOne<Box>("/");
    QList<Distribution*> distributions = root->findChildren<Distribution*>();
    for (Distribution *dist : distributions) {
        Box *parent = dist->port()->boxParent();
        if (parent == this) {
            ThrowException("SensitivityAnalysis cannot itself hold distributed inputs")
                .value(dist->objectName())
                .context(this);
        }
    }
    inputsAnalysed = distributions.size();
}

void SensitivityAnalysis::reset() {
    Box *root = findOne<Box>("/");
    QVector<Port*> ports = root->findMany<Port>("*[*]");
    int n = 0;
    for (Port *port : ports) {
        bool isInput = port->access()==PortAccess::Input,
            isInThisBox = port->boxParent()==this,
            isRoot = port->boxParent()==environment().root();
        if (isInput && !isInThisBox && !isRoot)
            ++n;
    }
    inputsTotal = n;
}

void SensitivityAnalysis::update() {
}


}
