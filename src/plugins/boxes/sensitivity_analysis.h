#ifndef SENSITIVITY_ANALYSIS_H
#define SENSITIVITY_ANALYSIS_H
#include <base/box.h>

namespace boxes {

class SensitivityAnalysis : public base::Box
{
public:
    SensitivityAnalysis(QString name, QObject *parent);
    void amend();
    void reset();
    void update();
private:
    // Inputs
    int iterations;
    QString method;
    // Outputs
    int inputsTotal, inputsAnalysed;
};

}

#endif
