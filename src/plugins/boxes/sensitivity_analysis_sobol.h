#ifndef SENSITIVITY_ANALYSIS_SOBOL_H
#define SENSITIVITY_ANALYSIS_SOBOL_H
#include <base/matrix.h>
#include "sensitivity_analysis_base.h"

namespace boxes {

class SensitivityAnalysisSobol : public SensitivityAnalysisBase
{
public:
    SensitivityAnalysisSobol(QString name, QObject *parent);
    void initialize();
    void reset();
private:
    // Input
    int iteration;
    // Output
    char phase;
    // Data
    base::Matrix<double> A, B, C;
    // Methods
    int numberOfIterations() const;
    int numberOfSamples() const;
    void fillMatrices();
};

}

#endif
