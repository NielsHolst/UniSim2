#ifndef APHID_OFFSPRING_H
#define APHID_OFFSPRING_H
#include <base/box.h>

namespace aphid {

class AphidOffspring : public base::Box
{
public:
    AphidOffspring(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double offspringFromSusceptible,
        offspringFromExposedApterous, offspringFromExposedAlate,
        aphidDensity, cropGrowthStage;
    // Outputs
    double alate, apterous, alateProportion;
};

}

#endif
