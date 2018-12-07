#ifndef SENSITIVITY_ANALYSIS_BASE_H
#define SENSITIVITY_ANALYSIS_BASE_H
#include <QVector>
#include <base/box.h>

namespace base {
    class Distribution;
    class Port;
}

namespace boxes {

class SensitivityAnalysisBase : public base::Box
{
public:
    SensitivityAnalysisBase(QString name, QObject *parent);
    void initialize();
protected:
    // Inputs
    int sampleSize;
    // Outputs
    int inputsTotal, inputsAnalysed, iterations;
    // Data
    QVector<base::Distribution*> _saDistributions;
private:
    // Methods
    virtual int numberOfIterations() const = 0;
    virtual int numberOfSamples() const = 0;
    bool canBeAnalysed(base::Port *port) const;
    void setInputsTotal();
    void setInputsAnalysed();
};

}

#endif
