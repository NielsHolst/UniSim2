#include <base/distribution.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/port.h>
#include <base/publish.h>
#include "sensitivity_analysis_sobol.h"

using namespace base;

namespace boxes {

PUBLISH(SensitivityAnalysisSobol)

SensitivityAnalysisSobol::SensitivityAnalysisSobol(QString name, QObject *parent)
    : SensitivityAnalysisBase(name, parent)
{
    help("runs a Sobol sensitivity analysis");
    Input(iteration).imports("/*[iteration]");
}

void SensitivityAnalysisSobol::initialize() {
    SensitivityAnalysisBase::initialize();
    _A.resize(sampleSize, inputsAnalysed);
    _B.resize(sampleSize, inputsAnalysed);
}

int SensitivityAnalysisSobol::numberOfIterations() const {
    return sampleSize*(2+inputsAnalysed);
}

void SensitivityAnalysisSobol::reset() {
    char phase = (iteration <= sampleSize) ? 'A' : (iteration <= 2*sampleSize) ? 'B' : 'C';
    if (phase < 'C') {
        Matrix<double> *M = (phase == 'A') ? &_A : &_B;
        int row = (iteration-1) % sampleSize,
            col = 0;
        for (Distribution *dist : _saDistributions) {
            double value = dist->draw();
            dist->port()->equals(value);
            (*M)(row,col++) = value;
        }
    }
    else {
        int row = (iteration-1) % sampleSize,
            colA = (iteration-1) / sampleSize - 2,
            colB = 0;
        for (Distribution *dist : _saDistributions) {
            double value = (colA == colB) ? _A.at(row, colA) : _B.at(row, colB);
            ++colB;
            dist->port()->equals(value);
        }
    }
}

int SensitivityAnalysisSobol::numberOfSamples() const {
    return 2*sampleSize;
}

}
