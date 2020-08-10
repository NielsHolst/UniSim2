#ifndef SPECIFIC_GROWTH_RATE_H
#define SPECIFIC_GROWTH_RATE_H
#include <base/box.h>

namespace saccharina {

class Specificgrowthrate : public base::Box
{
public:
    Specificgrowthrate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double farea, fphoto, ftemp, N, Nmin, C, Cmin, dt, Factor;
    // Outputs
    double specificgrowthrate;
};

}

#endif
