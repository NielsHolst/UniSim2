#ifndef SENSITIVITY_ANALYSIS_H
#define SENSITIVITY_ANALYSIS_H
#include <QVector>
#include <base/box.h>

namespace base {
    class Distribution;
    class Port;
}

namespace boxes {

class SensitivityAnalysis : public base::Box
{
public:
    SensitivityAnalysis(QString name, QObject *parent);
    void initialize();
    void reset();
private:
    // Inputs
    int iterations;
    QString method;
    // Outputs
    int inputsTotal, inputsAnalysed;
    // Data
    enum Method {MC, LHS};
    Method _method;
    QVector<base::Distribution*> _saDistributions;
    // Methods
    bool canBeAnalysed(base::Port *port) const;
    void setMethod();
    void setInputsTotal();
    void setInputsAnalysed();
};

}

#endif
