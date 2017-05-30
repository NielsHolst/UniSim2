#ifndef LOGUNIFORM_H
#define LOGUNIFORM_H
#include <base/distribution.h>
#include "log_parameter.h"

namespace distribution {

class loguniform : public base::Distribution
{
public:
    loguniform(QString name, QObject *parent);
    void min(double value);
    void max(double value);
    QPair<double,double> bounds() const;
    double inverse(double y) const;
private:
    // Parameters
    LogParameter _min, _max;
    // Methods
    void parseArguments();
};

}

#endif
