#ifndef NORMAL_H
#define NORMAL_H
#include <base/distribution.h>

namespace distribution {

class normal : public base::Distribution
{
public:
    normal(QString name, QObject *parent);
    void mean(double value);
    void sd(double value);
    void lowerQuantile(double value);
    void upperQuantile(double value);
    QPair<double,double> bounds() const;
    double inverse(double y) const;
private:
    // Parameters
    double _mean, _sd, _lowerQuantile, _upperQuantile;
    // Methods
    void parseArguments();
};

}

#endif
