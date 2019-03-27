#ifndef SENSITIVITY_ANALYSIS_SIMPLE_H
#define SENSITIVITY_ANALYSIS_SIMPLE_H
#include "sensitivity_analysis_base.h"


namespace boxes {

class SensitivityAnalysisSimple : public SensitivityAnalysisBase
{
public:
    SensitivityAnalysisSimple(QString name, QObject *parent);
    void reset();
private:
    // Inputs
    QString method;
    int simIterations;
    // Methods
    int numberOfIterations() const;
    int numberOfSamples() const;
};

}

#endif
