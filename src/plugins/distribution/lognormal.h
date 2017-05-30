#ifndef LOGNORMAL_H
#define LOGNORMAL_H
#include <base/distribution.h>
#include "log_parameter.h"

namespace distribution {

class lognormal : public base::Distribution
{
public:
    lognormal(QString name, QObject *parent);
    void mean(double value);
    void sd(double value);
    void lowerQuantile(double value);
    void upperQuantile(double value);
    QPair<double,double> bounds() const;
    double inverse(double y) const;
private:
    // Parameters
    LogParameter _mean;
    double _sd, _lowerQuantile, _upperQuantile;
    // Methods
    void parseArguments();
};

}

#endif
