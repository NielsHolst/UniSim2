#ifndef TRAPEZOID_H
#define TRAPEZOID_H
#include <base/box.h>

namespace saccharina {

class Trapezoid : public base::Box
{
public:
    Trapezoid(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double T, T0, Topt1, Topt2, Tmax, maxValue;
    // Outputs
    double value;
};

}

#endif
