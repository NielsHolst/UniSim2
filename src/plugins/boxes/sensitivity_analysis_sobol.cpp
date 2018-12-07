#include <base/dialog.h>
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
    Output(phase).help("Matrix used in this iteration (A, B or C)");
}

void SensitivityAnalysisSobol::initialize() {
    SensitivityAnalysisBase::initialize();
    fillMatrices();
}

namespace {
    void copyColumn(const Matrix<double> &a, Matrix<double> &b, int col) {
        for (int row=0; row<a.numRow(); ++row)
            b(row,col) = a.at(row,col);
    }
}

void SensitivityAnalysisSobol::reset() {
    phase = (iteration <= sampleSize) ? 'A' :
            (iteration <= 2*sampleSize) ? 'B' : 'C';
    int row = (iteration-1) % sampleSize,
        col = 0;
    dialog().information("Size = " + QString::number(A.numRow()) + ":" + QString::number(A.numCol()));
    switch (phase) {
    case 'A':
        for (Distribution *dist : _saDistributions) {
            dialog().information(QString::number(row) + ":" + QString::number(col));
            dist->port()->equals(A.at(row, col++));
            dialog().information("OK B");
        }
        dialog().information("OK C");
        break;
    case 'B':
        for (Distribution *dist : _saDistributions)
            dist->port()->equals(B.at(row, col++));
        break;
    case 'C':
        int colSwap = (iteration - 2*sampleSize)/inputsAnalysed;
        Q_ASSERT(colSwap < inputsAnalysed);
        if (colSwap > 1)
            copyColumn(B,C,colSwap-1);
        copyColumn(A,C,colSwap);
        for (Distribution *dist : _saDistributions)
            dist->port()->equals(C.at(row, col++));
    }
}

void SensitivityAnalysisSobol::fillMatrices() {
    A.resize(sampleSize, inputsAnalysed);
    B.resize(sampleSize, inputsAnalysed);
    C.resize(sampleSize, inputsAnalysed);
    Q_ASSERT(inputsAnalysed == _saDistributions.size());
    for (int row = 0; row < sampleSize; ++row) {
        int col = 0;
        for (Distribution *dist : _saDistributions) {
            A(row, col) = dist->draw();
            B(row, col) = dist->draw();
            C(row, col) = B(row, col);
            ++col;
        }
    }
}

int SensitivityAnalysisSobol::numberOfSamples() const {
    return 2*sampleSize;
}

int SensitivityAnalysisSobol::numberOfIterations() const {
    return sampleSize*(2+inputsAnalysed);
}

}
