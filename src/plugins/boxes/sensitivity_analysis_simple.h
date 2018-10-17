#ifndef SENSITIVITY_ANALYSIS_SIMPLE_H
#define SENSITIVITY_ANALYSIS_SIMPLE_H
#include "sensitivity_analysis_base.h"


namespace boxes {

class SensitivityAnalysisSimple : public SensitivityAnalysisBase
{
public:
    SensitivityAnalysisSimple(QString name, QObject *parent);
private:
    // Methods
    int numberOfIterations() const;
    int numberOfSamples() const;
};

}

#endif