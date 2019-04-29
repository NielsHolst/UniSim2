#ifndef APHID_JUVENILE_SURVIVAL_H
#define APHID_JUVENILE_SURVIVAL_H
#include <base/box.h>

namespace aphid {

class AphidJuvenileSurvival : public base::Box
{
public:
    AphidJuvenileSurvival(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double cropGrowthStage, temperature;
    // Outputs
    double value;
};

}

#endif
