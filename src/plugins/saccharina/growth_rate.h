#ifndef GROWTH_RATE_H
#define GROWTH_RATE_H
#include <base/box.h>

namespace saccharina {

class GrowthRate : public base::Box
{
public:
    GrowthRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double maxValue, farea, ftemp, fsalinity, fnitrogen, fcarbon;
    // Outputs
    double value;
};

}

#endif
