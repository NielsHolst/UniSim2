#ifndef POTATO_DEVELOPMENT_WE_H
#define POTATO_DEVELOPMENT_WE_H
#include <base/box.h>

namespace potato {

class PotatoDevelopmentWE: public base::Box
{
public:
    PotatoDevelopmentWE(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Tmin, Topt, Tmax,
           Pcrit, omega,
           rMaxVeg, rMaxTub, rMaxSen,
           T, P;
    // Outputs
    double stepVeg, stepTub, stepSen;
    // Data
    double _alpha;
    // Methods
    double fT() const;
    double fP() const;
    double p(double x, int n=1) const;
};

}
#endif
