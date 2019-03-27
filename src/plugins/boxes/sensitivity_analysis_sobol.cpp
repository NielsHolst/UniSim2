#include <base/distribution.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/port.h>
#include <base/publish.h>
#include "output_r.h"
#include "sensitivity_analysis_sobol.h"

using namespace base;

namespace boxes {

PUBLISH(SensitivityAnalysisSobol)

SensitivityAnalysisSobol::SensitivityAnalysisSobol(QString name, QObject *parent)
    : SensitivityAnalysisBase(name, parent),
      N(sampleSize), k(inputsAnalysed)
{
    help("runs a Sobol sensitivity analysis");
    Input(bootstrapSize).equals(1000).help("Size of bootstrap sample (cheap in computation time)");
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
    phase = (iteration <= N) ? 'A' :
            (iteration <= 2*N) ? 'B' : 'C';
    int row = (iteration-1) % N,
        col = 0;
    switch (phase) {
    case 'A':
        for (Distribution *dist : _saDistributions) {
            dist->port()->equals(A.at(row, col++));
        }
        break;
    case 'B':
        for (Distribution *dist : _saDistributions)
            dist->port()->equals(B.at(row, col++));
        break;
    case 'C':
        // Note that iteration counts from 1 not 0
        int colSwap = (iteration - 1)/N - 2;
        Q_ASSERT(colSwap>=0 && colSwap < k);
        if (colSwap > 0)
            copyColumn(B,C,colSwap-1);
        copyColumn(A,C,colSwap);
        for (Distribution *dist : _saDistributions)
            dist->port()->equals(C.at(row, col++));
    }
}

void SensitivityAnalysisSobol::fillMatrices() {
    A.resize(N, k);
    B.resize(N, k);
    C.resize(N, k);
    Q_ASSERT(k == _saDistributions.size());
    for (int row = 0; row < N; ++row) {
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
    return 2*N;
}

int SensitivityAnalysisSobol::numberOfIterations() const {
    return N*(2+k);
}

void SensitivityAnalysisSobol::debrief() {
//    environment().openOutputFile(_file, ".txt");
//    _stream.setDevice(&_file);
//    _stream << "k\n"
//            << k << "\n";
//    _file.close();
//    environment().incrementFileCounter();
    OutputR *outputR = findMaybeOne<OutputR>("*");
    if (outputR) {
        outputR->addRCode("sobol_k = " + QString::number(k) +
                          "; sobol_N = " + QString::number(N) +
                          "; sobol_B = " + QString::number(bootstrapSize) +"\n" +
                          "source(\"" + environment().inputFileNamePath("scripts/begin-sobol.R") + "\")"
                          );
    }
}

}
