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
}

int SensitivityAnalysisSimple::numberOfIterations() const {
    return sampleSize;
}

int SensitivityAnalysisSimple::numberOfSamples() const {
    return sampleSize;
}

}