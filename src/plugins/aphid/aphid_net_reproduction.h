#ifndef APHID_NET_REPRODUCTION_H
#define APHID_NET_REPRODUCTION_H
#include <base/box.h>

namespace aphid {

class AphidNetReproduction : public base::Box
{
public:
    AphidNetReproduction(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double Tmin, Tmax, Topt, R0opt, temperature,
        cropGrowthStage,
        optimumCropGrowthStageFrom, optimumCropGrowthStageTo, optimumCropFactor,
        alateFactor,
        aphidDensity, aphidDensityThreshold, exposureCost;
    // Outputs
    double apterous, alate,
        apterousExposed, alateExposed;
};

}

#endif
