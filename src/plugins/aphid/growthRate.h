#ifndef GROWTHRATE_H
#define GROWTHRATE_H
#include <base/box.h>

namespace aphid {

class GrowthRate : public base::Box
{
public:
    GrowthRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double growth_rate_season;
    double growth_rate_senescence;
    double proportion_senescence;
    // Outputs
    double growthRate_t;
};

}

#endif
