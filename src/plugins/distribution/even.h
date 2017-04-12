#ifndef EVEN_H
#define EVEN_H
#include <base/distribution.h>

namespace distribution {

class even : public base::Distribution
{
public:
    even(QString name, QObject *parent);
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
