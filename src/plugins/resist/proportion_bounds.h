#ifndef PROPORTION_BOUNDS_H
#define PROPORTION_BOUNDS_H
#include <base/box.h>

namespace resist {

class ProportionBounds : public base::Box
{
public:
    ProportionBounds(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double logP, logWidth;
    // Outputs
    double Pmin, Pmax;
};

}

#endif
