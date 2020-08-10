#ifndef SEASONAL_INFLUENCE_ON_GROWTHRATE_H
#define SEASONAL_INFLUENCE_ON_GROWTHRATE_H
#include <base/box.h>

namespace saccharina {

class Seasonalinfluenceongrowthrate : public base::Box
{
public:
    Seasonalinfluenceongrowthrate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double a1, a2, dayLength, maxDayLengthChange;
    // Outputs
    double dayLengthChange, lambda, fphoto;
    // Data
    double prevDayLength;
};

}

#endif
