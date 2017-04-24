#ifndef UNIFORM_H
#define UNIFORM_H
#include <base/distribution.h>

namespace distribution {

class uniform : public base::Distribution
{
public:
    uniform(QString name, QObject *parent);
    void min(double value);
    void max(double value);
    QPair<double,double> bounds() const;
    double inverse(double y) const;
private:
    // Parameters
    double _min, _max;
    // Methods
    void parseArguments();
};

}

#endif
